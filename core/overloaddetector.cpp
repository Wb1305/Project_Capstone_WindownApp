#include "overloaddetector.h"
#include <QDebug>
#include <QTimer>

#define OVERLOADING 50
#define WARNING 45

// weights
#define WEIGHT_RAM 0.35
#define WEIGHT_SWAP 0.25
#define WEIGHT_CPU 0.25
#define WEIGHT_TEMP 0.10
#define WEIGHT_FREQ 0.05

#define TEMP_MIN 40
#define TEMP_MAX 90

// Critical thresholds
#define CRITICAL_CPU_THRESHOULD 85
#define CRITICAL_MEM_THRESHOULD 180
#define CRITICAL_TEMP_THRESHOULD 90

// --- Overload config ---
#define OVERLOAD_COUNT_THRESHOLD      40
#define OVERLOAD_CONSECUTIVE_THRESHOLD 30
#define OVERLOAD_DEBOUNCE_MS         10000

// --- Warning config ---
#define WARNING_COUNT_THRESHOLD       20
#define WARNING_CONSECUTIVE_THRESHOLD 15
#define WARNING_DEBOUNCE_MS          10000

// --- Potential Overload ---
#define POTENTIAL_OVERLOAD_COUNT      45


OverloadDetector::OverloadDetector(QObject *parent)
    : QObject{parent}
{
    m_lastOverloadSignal.start();
    m_lastWarningSignal.start();
}

int OverloadDetector::detectState(const SystemStats &systemStats)
{
    // SystemMetrics metrics = getMetrics(systemStats);
    double cpuTemp = systemStats.cpuStats().general().temperature();
    double cpuUsage = systemStats.cpuStats().general().utilization();
    double ramUsagePercent = systemStats.memStats().ramPercent();
    double swapUsagePercent = systemStats.memStats().swapPercent();
    double cpuFreqPercent = systemStats.cpuStats().general().frequencyPercent();

    double normTemp = std::clamp((cpuTemp - TEMP_MIN) / (TEMP_MAX - TEMP_MIN)*100, 0.0, 100.0);

    if(isCriticalOverloading(cpuUsage, ramUsagePercent + swapUsagePercent, normTemp)) {
        qDebug() << "[OverloadDetector] Critical condition detected!";
        qDebug() << "CPU:" << cpuUsage
                 << "RAM:" <<  ramUsagePercent
                 << "SWAP:" << swapUsagePercent
                 << "Temp:" << normTemp;
        return (int)LoadLevel::STATE_OVERLOADED;
    }

    double score =
        WEIGHT_RAM * ramUsagePercent +
        WEIGHT_SWAP * swapUsagePercent +
        WEIGHT_CPU * cpuUsage +
        WEIGHT_TEMP * normTemp +
        WEIGHT_FREQ * cpuFreqPercent;
    score += balancePenalty(systemStats.cpuStats().cores());

    // printValidateLoading(systemStats, normTemp, score);

    return validLoading(score);
}

void OverloadDetector::evaluateOverloadTrend(int currentState)
{
    qDebug()<<"[OverloadDetector] ===== Evaluate Overload Trend =====";
    updateStateHistory(currentState);

    int overloadCount = 0;
    int warningCount = 0;
    countStateOccurrences(overloadCount, warningCount);

    updateConsecutiveCounts(currentState);

    checkAndEmitSignals(overloadCount, warningCount);

    printStateHistory();
}

bool OverloadDetector::isCriticalOverloading(double cpuUsage, double memUsage, double normTemp)
{
    return cpuUsage >= CRITICAL_CPU_THRESHOULD || memUsage >= CRITICAL_MEM_THRESHOULD || normTemp >= CRITICAL_TEMP_THRESHOULD;
}

double OverloadDetector::balancePenalty(const QVector<CpuCore> &cores)
{
    double coreImbalance = calCoreImbalance(cores);
    if(coreImbalance > 0.5) {
        qDebug() << "Core imbalance detected:" << coreImbalance
                 << "Penalty added:" << coreImbalance*10;
        return coreImbalance*10;
    }
    return 0.0;
}

double OverloadDetector::calCoreImbalance(const QVector<CpuCore> &cores)
{
    double maxCoreUsage = 0.0;
    double minCoreUsage = 100.0;
    for(const auto &core: cores) {
        double coreUsage = core.utilization();
        maxCoreUsage = std::max(maxCoreUsage, coreUsage);
        minCoreUsage = std::min(minCoreUsage, coreUsage);
    }
    double coreImbalance = (maxCoreUsage - minCoreUsage)/100.0;
    return coreImbalance;
}

void OverloadDetector::printValidateLoading(const SystemStats &systemStats, double normTemp, double score)
{
    qDebug() << "[OverloadDetector] System Status:"
             << "\n  CPU Usage:" << systemStats.cpuStats().general().utilization() << "%"
             << "\n  RAM Usage:" << systemStats.memStats().ramPercent() << "%"
             << "\n  SWAP Usage:" << systemStats.memStats().swapPercent()<< "%"
             << "\n  Temperature:" << normTemp << "%"
             << "\n  Final Score:" << score;
}

int OverloadDetector::validLoading(double &score)
{
    if(score >= OVERLOADING) return (int)LoadLevel::STATE_OVERLOADED;
    else if (score >= WARNING) return (int)LoadLevel::STATE_WARNING;
    return (int)LoadLevel::STATE_NORMAL;
}

void OverloadDetector::updateStateHistory(int state)
{
    if (m_stateHistory.size() >= 60){
        m_stateHistory.dequeue();
    }
    m_stateHistory.enqueue(state);
}

void OverloadDetector::countStateOccurrences(int &overloadCount, int &warningCount)
{
    overloadCount = std::count(m_stateHistory.begin(), m_stateHistory.end(), (int)LoadLevel::STATE_OVERLOADED);
    warningCount = std::count(m_stateHistory.begin(), m_stateHistory.end(), (int)LoadLevel::STATE_WARNING);
}

void OverloadDetector::updateConsecutiveCounts(int currentState)
{
    if(currentState == (int)LoadLevel::STATE_OVERLOADED){
        m_consecutiveOverload++;
        m_consecutiveWarning = 0;
        return;
    }

    if(currentState == (int)LoadLevel::STATE_WARNING){
        m_consecutiveWarning++;
        m_consecutiveOverload = 0;
        return;
    }

    m_consecutiveOverload = 0;
    m_consecutiveWarning = 0;
}

void OverloadDetector::checkAndEmitSignals(int overloadCount, int warningCount)
{
    if (overloadCount >= OVERLOAD_COUNT_THRESHOLD &&
        m_consecutiveOverload >= OVERLOAD_CONSECUTIVE_THRESHOLD &&
        m_lastOverloadSignal.elapsed() > OVERLOAD_DEBOUNCE_MS)
    {
        qDebug()<<"[OverloadDetector] ==== OVERLOADING ===";
        emit overloadDetected();
        emit overloadDetectedWithBuffer(m_snapshotBuffer);
        m_lastOverloadSignal.restart();
        m_snapshotBuffer.clear();
        return;
    }

    if (warningCount >= WARNING_COUNT_THRESHOLD &&
        m_consecutiveWarning >= WARNING_CONSECUTIVE_THRESHOLD &&
        m_lastWarningSignal.elapsed() > OVERLOAD_DEBOUNCE_MS)
    {
        qDebug()<<"[OverloadDetector] ==== WARNING ===";
        emit warningDetected();
        m_lastWarningSignal.restart();
        return;
    }

    if (overloadCount >= POTENTIAL_OVERLOAD_COUNT &&
        m_consecutiveOverload < OVERLOAD_CONSECUTIVE_THRESHOLD)
    {
        qDebug() << "[Warning] Potential Overload: High frequency but not continuous.";
        // emit potentialOverload(); // nếu cần thiết
        return;
    }

    if (m_consecutiveOverload == 0 && m_consecutiveWarning == 0) {
        qDebug()<<"[OverloadDetector] ==== NORMAL ===";
        emit systemNormal();
    }
}

void OverloadDetector::printStateHistory() const
{
    qDebug() << "[OverloadDetector] Last 60 States (0:Normal, 1:Warning, 2:Overloaded):";
    int idx = 1;
    for (int state : m_stateHistory)
    {
        qDebug() << QString(" %1. State = %2").arg(idx++).arg(state);
    }
}

void OverloadDetector::recordSnapshot(const SystemStats &systemStats, const QVector<ProcessInfo> &processes, int detectedState)
{
    OverloadSnapshot snapShot;
    snapShot.m_timestamp = systemStats.timestamp();
    snapShot.m_systemStats = systemStats;
    snapShot.m_processes = processes;
    snapShot.detectedState = detectedState;

    m_snapshotBuffer.enqueue(snapShot);
    if(m_snapshotBuffer.size() > 60){
        m_snapshotBuffer.dequeue();
    }
}

void OverloadDetector::onSystemDataReceived(const SystemStats &systemStats, const QVector<ProcessInfo> &processes)
{
    qDebug()<< "[Thread - OverloadDetector]: Overload received data from System Monitor";

    int currState = detectState(systemStats);
    evaluateOverloadTrend(currState);

    recordSnapshot(systemStats, processes, currState);

    // printValidateLoading(systemStats, 60, 8);


    // test timer
    // static bool overloadTimerStarted = false;

    // if (!overloadTimerStarted) {
    //     overloadTimerStarted = true;

    //     qDebug() << "[Thread - OverloadDetector]: Starting 20s overload timer...";

    //     QTimer::singleShot(20000, this, [this]() {
    //         qDebug() << "[Thread - OverloadDetector]: emit signal overload after 20s";
    //         emit overloadDetected();
    //     });
    // }
}

