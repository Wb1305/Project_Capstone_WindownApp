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

OverloadDetector::OverloadDetector(QObject *parent)
    : QObject{parent}
{}

int OverloadDetector::detectState(SystemStats &systemStats)
{
    // SystemMetrics metrics = getMetrics(systemStats);
    double cpuTemp = systemStats.cpuStats().general().temperature();
    double cpuUsage = systemStats.cpuStats().general().utilization();
    double ramUsagePercent = systemStats.memStats().ramPercent();
    double swapUsagePercent = systemStats.memStats().swapPercent();
    double cpuFreqPercent = systemStats.cpuStats().general().frequencyPercent();

    double normTemp = std::clamp((cpuTemp - TEMP_MIN) / (TEMP_MAX - TEMP_MIN)*100, 0.0, 100.0);

    if(isCriticalOverloading(cpuUsage, ramUsagePercent + swapUsagePercent, normTemp)) {
        qDebug() << "Critical condition detected!";
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
    printValidateLoading(systemStats, normTemp, score);

    return validLoading(score);
}

bool OverloadDetector::checkOverload(double currentCpu, double currentRam) const
{
    return true;
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
    qDebug() << "System Status:"
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

void OverloadDetector::updateUsage(const SystemStats &systemStats)
{
    qDebug()<< "Thread: Overload updated data";
    printValidateLoading(systemStats, 60, 8);

    QTimer::singleShot(20000, this, [this](){
        qDebug()<< "Thread: emit signal overload after 20s";
        emit overloadDetected();
    });
}

// void OverloadDetector::updateUsage(double currentCpu, double currentRam)
// {
//     qDebug()<<"CPU: "<<currentCpu<<", Ram: "<<currentRam;
// }
