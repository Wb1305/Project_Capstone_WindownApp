#ifndef OVERLOADDETECTOR_H
#define OVERLOADDETECTOR_H

// #include "model/systemstats.h"
// #include "model/processinfo.h"
#include "model/systemstats.h"
#include <QObject>
#include <QVector>

class OverloadDetector : public QObject
{
    Q_OBJECT
public:
    explicit OverloadDetector(QObject *parent = nullptr);

    enum class LoadLevel {
        STATE_NORMAL = 0,
        STATE_WARNING,
        STATE_OVERLOADED
    };

    int detectState(SystemStats &systemStats);

private:
    bool checkOverload(double currentCpu, double currentRam) const;
    bool isCriticalOverloading(double cpuUsage, double memUsage, double normTemp);
    double balancePenalty(const QVector<CpuCore> &cores);
    double calCoreImbalance(const QVector<CpuCore> &cores);
    void printValidateLoading(const SystemStats &systemStats, double normTemp, double score);
    int validLoading(double &score);

signals:
    void overloadDetected();
    // void overloadDetectedWithProcesses(const QVector<ProcessInfo>& processList);
public slots:
    // void updateUsage(double currentCpu, double currentRam);
    void updateUsage(const SystemStats &systemStats);

};

#endif // OVERLOADDETECTOR_H
