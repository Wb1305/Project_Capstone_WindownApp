#ifndef ISYSTEMMONITOR_H
#define ISYSTEMMONITOR_H

#include "model/processinfo.h"
#include "model/systemstats.h"
#include<QObject>

class ISystemMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ISystemMonitor(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ISystemMonitor() = default;

    virtual void startMonitoring() = 0;
    virtual void stopMonitoring() = 0;

    virtual SystemStats getCurrentSystemStats() const = 0;
    virtual QList<ProcessInfo> getCurrentProcesses() const = 0;

signals:
    void systemUpdated(const SystemStats& systemStats, const QList<ProcessInfo>& processes);
};

#endif // ISYSTEMMONITOR_H
