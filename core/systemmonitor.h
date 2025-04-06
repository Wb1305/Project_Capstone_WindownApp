#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include "ISystemMonitor.h"
#include "model/processinfo.h"

#include <QObject>

class SystemMonitor : ISystemMonitor
{
    Q_OBJECT
public:
    // SystemMonitor();
    explicit SystemMonitor(QObject* parent = nullptr);

    void startMonitoring() override;
    void stopMonitoring() override;

    SystemStats getCurrentSystemStats() const override;
    QList<ProcessInfo> getCurrentProcesses() const override;

    void generateFakeData();

// private slots:
//     void onDataReceived(const QByteArray& rawData);

private:
    // TcpServer* m_tcpServer = nullptr;
    // DataProcessor* m_processor = nullptr;
    // OverloadDetector* m_detector = nullptr;
    // ConfigManager* m_config = nullptr;

    SystemStats m_systemStats;
    QList<ProcessInfo> m_processList;
};

#endif // SYSTEMMONITOR_H
