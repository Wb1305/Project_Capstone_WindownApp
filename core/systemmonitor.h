#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include "ISystemMonitor.h"
#include "core/configmanager.h"
#include "core/dataprocessor.h"
#include "core/ivisocketserver.h"
#include "core/overloaddetector.h"
#include "core/processmanager.h"
#include "model/processinfo.h"

#include <QObject>

class SystemMonitor : public QObject, public ISystemMonitor
{
    Q_OBJECT
public:
    // SystemMonitor();
    explicit SystemMonitor(QObject* parent = nullptr);

    void startMonitoring() override;
    void stopMonitoring() override;

    SystemStats getCurrentSystemStats() const override;
    QVector<ProcessInfo> getCurrentProcesses() const override;

    void bindToDataProcessor(DataProcessor* processor);
    void setOverloadDetector(OverloadDetector* detector);
    void setProcessManager(ProcessManager* procManager);
    void setConfigManager(ConfigManager* confManager);

    ConfigManager* configManager() const;

    // --- tạo data giả lập ---
    QByteArray generateFakeData();
    void printParsedData(const SystemStats& systemStats, const QVector<ProcessInfo>& processes);
    // --- cung cấp data giả ---
    void feedFakeData(const QByteArray& fakeData);

    Q_INVOKABLE void startStressTest(int numberOfTasks, int memUsagePercent, int numberOfCores, int timeout);
    Q_INVOKABLE void stopStressTest();

    QByteArray createCommandForStressTestJson(int numberOfTasks, int memUsagePercent, int numberOfCores, int timeout);
    QByteArray createCommandStopStressJson();
    QByteArray createCommandKillProcessJson(const QString &procName);
    QByteArray createCommandHandleWarningState();

signals:
    void systemUpdated(const SystemStats& systemStats, const QVector<ProcessInfo>& processes);
    // void systemUsageChanged(double cpuPercent, double memPercent);
    void systemUsageChanged(const SystemStats& systemStats);
    void processListReady(const QVector<ProcessInfo>& procList);
    void commandReceived(const QByteArray &commandJson);
    void informOverloadInfoForUI(const QVariantMap &info);

private slots:
    void onDataReceived(const QByteArray& rawData);
    void onOverloadDetected();
    void onCommandKillProcessReceived(const QString &procName);
    void onWarningStateReceived();
    void onInformOverloadInfoForUI(const QVariantMap &info);

private:
    IviSocketServer* m_iviServer = nullptr;
    DataProcessor* m_dataProcessor = nullptr;
    OverloadDetector* m_detector = nullptr;
    ConfigManager* m_config = nullptr;
    ProcessManager* m_processManager = nullptr;

    SystemStats m_systemStats;
    QVector<ProcessInfo> m_processList;
};

#endif // SYSTEMMONITOR_H
