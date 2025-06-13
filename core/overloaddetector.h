#ifndef OVERLOADDETECTOR_H
#define OVERLOADDETECTOR_H

// #include "model/systemstats.h"
// #include "model/processinfo.h"
#include "core/configmanager.h"
#include "model/OverloadConfig.h"
#include "model/OverloadSnapshot.h"
#include "model/systemstats.h"
#include <QObject>
#include <QVector>
#include <QQueue>
#include <qelapsedtimer.h>
#include <QQueue>

struct DetectResult {
    int state;
    double score;
    double balancePenaty;
};

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

    // int detectState(const SystemStats &systemStats);
    DetectResult detectState(const SystemStats &systemStats);
    void evaluateOverloadTrend(int currentState);
    void setConfigManager(ConfigManager* configManager);

    void printOverloadConfig() const;

private:
    bool isCriticalOverloading(double cpuUsage, double memUsage, double normTemp);
    double balancePenalty(const QVector<CpuCore> &cores);
    double calCoreImbalance(const QVector<CpuCore> &cores);
    void printValidateLoading(const SystemStats &systemStats, double normTemp, double score);
    int validLoading(double &score);

    //=========
    void updateStateHistory(int state);
    void countStateOccurrences(int &overloadCount, int &warningCount);
    void updateConsecutiveCounts(int currentState);
    void checkAndEmitSignals(int overloadCount, int warningCount);

    //=========
    void printStateHistory() const;

    void recordSnapshot(const SystemStats &systemStats, const QVector<ProcessInfo> &processes,
                        int detectedState, double score, double balancePenatyVal);

    //=========

    void reloadConfigFromManager();

signals:
    void overloadDetected();
    void warningDetected();
    void systemNormal();
    void overloadDetectedWithBuffer(const QQueue<OverloadSnapshot> &buffer);
    void overloadMetricsAvailable(const SystemStats &systemStats, int state, double score, double balancePenatyVal);

public slots:
    // void updateUsage(double currentCpu, double currentRam);
    void onSystemDataReceived(const SystemStats &systemStats, const QVector<ProcessInfo> &processes);

private:
    int m_consecutiveOverload = 0; // chuổi overload liên tục
    int m_consecutiveWarning = 0; // chuổi warning liên tục

    QElapsedTimer m_lastOverloadSignal;
    QElapsedTimer m_lastWarningSignal;

    QQueue<int> m_stateHistory;
    QQueue<OverloadSnapshot> m_snapshotBuffer;

    OverloadConfig m_overloadConfig; // chứa các tham số
    ConfigManager* m_configManager = nullptr;
};

#endif // OVERLOADDETECTOR_H
