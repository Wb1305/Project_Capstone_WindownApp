#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include "ISystemMonitor.h"
#include "core/dataprocessor.h"
#include "core/ivisocketserver.h"
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

    void setDataProcessor(DataProcessor* processor);
    void generateFakeData();    
    void printParsedData(const SystemStats& systemStats, const QVector<ProcessInfo>& processes);

signals:
    void systemUpdated(const SystemStats& systemStats, const QVector<ProcessInfo>& processes);

private slots:
    void onDataReceived(const QByteArray& rawData);

private:
    IviSocketServer* m_iviServer = nullptr;
    DataProcessor* m_dataProcessor = nullptr;
    // OverloadDetector* m_detector = nullptr;
    // ConfigManager* m_config = nullptr;

    SystemStats m_systemStats;
    QVector<ProcessInfo> m_processList;
};

#endif // SYSTEMMONITOR_H
