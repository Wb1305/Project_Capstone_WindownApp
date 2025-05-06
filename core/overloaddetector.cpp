#include "overloaddetector.h"
#include <QDebug>
#include <QTimer>
#include <qthread.h>

// #define OVERLOADING 50
// #define WARNING 45

// // weights
// #define WEIGHT_RAM 0.35
// #define WEIGHT_SWAP 0.25
// #define WEIGHT_CPU 0.25
// #define WEIGHT_TEMP 0.10
// #define WEIGHT_FREQ 0.05

// #define TEMP_MIN 40
// #define TEMP_MAX 90

// // Critical thresholds
// #define CRITICAL_CPU_THRESHOULD 85
// #define CRITICAL_MEM_THRESHOULD 180
// #define CRITICAL_TEMP_THRESHOULD 90

// // --- Overload config ---
// #define OVERLOAD_COUNT_THRESHOLD     40
// #define OVERLOAD_CONSECUTIVE_THRESHOLD 30
// #define OVERLOAD_DEBOUNCE_MS         10000

// // --- Warning config ---
// #define WARNING_COUNT_THRESHOLD       20
// #define WARNING_CONSECUTIVE_THRESHOLD 15
// #define WARNING_DEBOUNCE_MS          10000

// // --- Potential Overload ---
// #define POTENTIAL_OVERLOAD_COUNT      45




OverloadDetector::OverloadDetector(QObject *parent)
    : QObject{parent}
{
    m_lastOverloadSignal.start();
    m_lastWarningSignal.start();
    // qDebug() << "[OverloadDetector] Created in thread:" << QThread::currentThread();
}

int OverloadDetector::detectState(const SystemStats &systemStats)
{
    // SystemMetrics metrics = getMetrics(systemStats);
    double cpuTemp = systemStats.cpuStats().general().temperature();
    double cpuUsage = systemStats.cpuStats().general().utilization();
    double ramUsagePercent = systemStats.memStats().ramPercent();
    double swapUsagePercent = systemStats.memStats().swapPercent();
    double cpuFreqPercent = systemStats.cpuStats().general().frequencyPercent();

    double normTemp = std::clamp((cpuTemp - m_overloadConfig.tempMin) / (m_overloadConfig.tempMax - m_overloadConfig.tempMin)*100, 0.0, 100.0);

    if(isCriticalOverloading(cpuUsage, ramUsagePercent + swapUsagePercent, normTemp)) {
        qDebug() << "[OverloadDetector] Critical condition detected!";
        qDebug() << "CPU:" << cpuUsage
                 << "RAM:" <<  ramUsagePercent
                 << "SWAP:" << swapUsagePercent
                 << "Temp:" << normTemp;
        return (int)LoadLevel::STATE_OVERLOADED;
    }

    double score =
        m_overloadConfig.weightRam * ramUsagePercent +
        m_overloadConfig.weightSwap * swapUsagePercent +
        m_overloadConfig.weightCpu * cpuUsage +
        m_overloadConfig.weightTemp * normTemp +
        m_overloadConfig.weightFreq * cpuFreqPercent;
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

void OverloadDetector::setConfigManager(ConfigManager *configManager)
{
    qDebug() << "[OverloadDetector] setConfigManager in thread:" << QThread::currentThread();

    if(!configManager) return;
    m_configManager = configManager;

    // Lần đầu tiên, load toàn bộ
    reloadConfigFromManager();

    printOverloadConfig();

    connect(configManager, &ConfigManager::configChanged, this, [this]() {
        printOverloadConfig();
    }, Qt::QueuedConnection);

    // connect(configManager, &ConfigManager::configChanged, this, &OverloadDetector::printOverloadConfig, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::overloadingValueChanged, this, [this]() {
        m_overloadConfig.overloadingValue = m_configManager->overloadingValue();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::warningValueChanged, this, [this]() {
        m_overloadConfig.warningValue = m_configManager->warningValue();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::weightRamChanged, this, [this]() {
        m_overloadConfig.weightRam = m_configManager->weightRam();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::weightSwapChanged, this, [this]() {
        m_overloadConfig.weightSwap = m_configManager->weightSwap();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::weightCpuChanged, this, [this]() {
        m_overloadConfig.weightCpu = m_configManager->weightCpu();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::weightTempChanged, this, [this]() {
        m_overloadConfig.weightTemp = m_configManager->weightTemp();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::weightFreqChanged, this, [this]() {
        m_overloadConfig.weightFreq = m_configManager->weightFreq();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::tempMinChanged, this, [this]() {
        m_overloadConfig.tempMin = m_configManager->tempMin();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::tempMaxChanged, this, [this]() {
        m_overloadConfig.tempMax = m_configManager->tempMax();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::criticalCpuThresholdChanged, this, [this]() {
        m_overloadConfig.criticalCpuThreshold = m_configManager->criticalCpuThreshold();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::criticalMemThresholdChanged, this, [this]() {
        m_overloadConfig.criticalMemThreshold = m_configManager->criticalMemThreshold();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::criticalTempThresholdChanged, this, [this]() {
        m_overloadConfig.criticalTempThreshold = m_configManager->criticalTempThreshold();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::criticalDurationSecondsThresholdChanged, this, [this]() {
        m_overloadConfig.criticalDurationSecondsThreshold = m_configManager->criticalDurationSecondsThreshold();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::overloadCountThresholdChanged, this, [this]() {
        m_overloadConfig.overloadCountThreshold = m_configManager->overloadCountThreshold();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::overloadConsecutiveThresholdChanged, this, [this]() {
        m_overloadConfig.overloadConsecutiveThreshold = m_configManager->overloadConsecutiveThreshold();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::overloadDebounceSecondsChanged, this, [this]() {
        m_overloadConfig.overloadDebounceSeconds = m_configManager->overloadDebounceSeconds();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::warningCountThresholdChanged, this, [this]() {
        m_overloadConfig.warningCountThreshold = m_configManager->warningCountThreshold();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::warningConsecutiveThresholdChanged, this, [this]() {
        m_overloadConfig.warningConsecutiveThreshold = m_configManager->warningConsecutiveThreshold();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::warningDebounceSecondsChanged, this, [this]() {
        m_overloadConfig.warningDebounceSeconds = m_configManager->warningDebounceSeconds();
    }, Qt::QueuedConnection);

    connect(configManager, &ConfigManager::potentialOverloadCountChanged, this, [this]() {
        // qDebug() << "potentialOverloadCountChanged Received configChanged in thread:" << QThread::currentThread();
        m_overloadConfig.potentialOverloadCount = m_configManager->potentialOverloadCount();
    }, Qt::QueuedConnection);
}

void OverloadDetector::printOverloadConfig() const
{
    qDebug() << "========== [OverloadDetector] Current OverloadConfig ==========";

    qDebug() << "Score thresholds:";
    qDebug() << "  Overloading Value:" << m_overloadConfig.overloadingValue;
    qDebug() << "  Warning Value    :" << m_overloadConfig.warningValue;

    qDebug() << "Weights (%):";
    qDebug() << "  RAM   :" << m_overloadConfig.weightRam;
    qDebug() << "  Swap  :" << m_overloadConfig.weightSwap;
    qDebug() << "  CPU   :" << m_overloadConfig.weightCpu;
    qDebug() << "  Temp  :" << m_overloadConfig.weightTemp;
    qDebug() << "  Freq  :" << m_overloadConfig.weightFreq;

    qDebug() << "Temperature Bounds:";
    qDebug() << "  Temp Min:" << m_overloadConfig.tempMin;
    qDebug() << "  Temp Max:" << m_overloadConfig.tempMax;

    qDebug() << "Critical thresholds:";
    qDebug() << "  CPU Usage (%):" << m_overloadConfig.criticalCpuThreshold;
    qDebug() << "  Mem Usage (MB):" << m_overloadConfig.criticalMemThreshold;
    qDebug() << "  Temp (°C)    :" << m_overloadConfig.criticalTempThreshold;
    qDebug() << "  Duration (s) :" << m_overloadConfig.criticalDurationSecondsThreshold;

    qDebug() << "Overload detection:";
    qDebug() << "  Count Threshold:" << m_overloadConfig.overloadCountThreshold;
    qDebug() << "  Consecutive Threshold:" << m_overloadConfig.overloadConsecutiveThreshold;
    qDebug() << "  Debounce (seconds):" << m_overloadConfig.overloadDebounceSeconds;

    qDebug() << "Warning detection:";
    qDebug() << "  Count Threshold:" << m_overloadConfig.warningCountThreshold;
    qDebug() << "  Consecutive Threshold:" << m_overloadConfig.warningConsecutiveThreshold;
    qDebug() << "  Debounce (seconds):" << m_overloadConfig.warningDebounceSeconds;

    qDebug() << "Potential Overload Detection:";
    qDebug() << "  Potential Overload Count:" << m_overloadConfig.potentialOverloadCount;

    qDebug() << "===============================================================";
}

bool OverloadDetector::isCriticalOverloading(double cpuUsage, double memUsage, double normTemp)
{
    // return cpuUsage >= CRITICAL_CPU_THRESHOULD || memUsage >= CRITICAL_MEM_THRESHOULD || normTemp >= CRITICAL_TEMP_THRESHOULD;
    return cpuUsage >= m_overloadConfig.criticalCpuThreshold || memUsage >= m_overloadConfig.criticalMemThreshold || normTemp >= m_overloadConfig.criticalTempThreshold;
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
    if(score >= m_overloadConfig.overloadingValue) return (int)LoadLevel::STATE_OVERLOADED; // OVERLOADING
    else if (score >= m_overloadConfig.warningValue) return (int)LoadLevel::STATE_WARNING; // WARNING
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
    if (overloadCount >= m_overloadConfig.overloadCountThreshold &&
        m_consecutiveOverload >= m_overloadConfig.overloadConsecutiveThreshold &&
        m_lastOverloadSignal.elapsed() > m_overloadConfig.overloadDebounceSeconds * 1000) //ms
    {
        qDebug()<<"[OverloadDetector] ==== OVERLOADING ===";
        emit overloadDetected();
        emit overloadDetectedWithBuffer(m_snapshotBuffer);

        qDebug()<<"[OverloadDetector] Clear SnapShot Buffer - State history";
        m_snapshotBuffer.clear();
        m_stateHistory.clear();
        m_consecutiveOverload = 0;
        m_consecutiveWarning = 0;
        m_lastOverloadSignal.restart();
        return;
    }

    if (warningCount >= m_overloadConfig.warningCountThreshold &&
        m_consecutiveWarning >= m_overloadConfig.warningConsecutiveThreshold &&
        m_lastWarningSignal.elapsed() > m_overloadConfig.warningDebounceSeconds * 1000) // ms
    {
        qDebug()<<"[OverloadDetector] ==== WARNING ===";
        emit warningDetected();
        m_lastWarningSignal.restart();
        return;
    }

    if (overloadCount >= m_overloadConfig.potentialOverloadCount &&
        m_consecutiveOverload < m_overloadConfig.overloadConsecutiveThreshold)
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

void OverloadDetector::reloadConfigFromManager()
{
    if(!m_configManager) return;

    qDebug() << "[OverloadDetector] Reloading config in thread:" << QThread::currentThread();

    m_overloadConfig.overloadingValue = m_configManager->overloadingValue();
    m_overloadConfig.warningValue = m_configManager->warningValue();

    m_overloadConfig.weightRam = m_configManager->weightRam();
    m_overloadConfig.weightSwap = m_configManager->weightSwap();
    m_overloadConfig.weightCpu = m_configManager->weightCpu();
    m_overloadConfig.weightTemp = m_configManager->weightTemp();
    m_overloadConfig.weightFreq = m_configManager->weightFreq();

    m_overloadConfig.tempMin = m_configManager->tempMin();
    m_overloadConfig.tempMax = m_configManager->tempMax();

    m_overloadConfig.criticalCpuThreshold = m_configManager->criticalCpuThreshold();
    m_overloadConfig.criticalMemThreshold = m_configManager->criticalMemThreshold();
    m_overloadConfig.criticalTempThreshold = m_configManager->criticalTempThreshold();
    m_overloadConfig.criticalDurationSecondsThreshold = m_configManager->criticalDurationSecondsThreshold();

    m_overloadConfig.overloadCountThreshold = m_configManager->overloadCountThreshold();
    m_overloadConfig.overloadConsecutiveThreshold = m_configManager->overloadConsecutiveThreshold();
    m_overloadConfig.overloadDebounceSeconds = m_configManager->overloadDebounceSeconds();

    m_overloadConfig.warningCountThreshold = m_configManager->warningCountThreshold();
    m_overloadConfig.warningConsecutiveThreshold = m_configManager->warningConsecutiveThreshold();
    m_overloadConfig.warningDebounceSeconds = m_configManager->warningDebounceSeconds();

    m_overloadConfig.potentialOverloadCount = m_configManager->potentialOverloadCount();

    qDebug() << "[OverloadDetector] Reloaded config from ConfigManager.";
    qDebug() << "[OverloadDetector] Done reloading config.";
}

void OverloadDetector::onSystemDataReceived(const SystemStats &systemStats, const QVector<ProcessInfo> &processes)
{
    qDebug()<< "[Thread - OverloadDetector]: Overload received data from System Monitor";

    int currState = detectState(systemStats);
    evaluateOverloadTrend(currState);

    recordSnapshot(systemStats, processes, currState);

    // printValidateLoading(systemStats, 60, 8);
}

