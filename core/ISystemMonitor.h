#ifndef ISYSTEMMONITOR_H
#define ISYSTEMMONITOR_H

#include "model/processinfo.h"
#include "model/systemstats.h"

class ISystemMonitor
{
public:
    virtual ~ISystemMonitor() = default;
    virtual void startMonitoring() = 0;
    virtual void stopMonitoring() = 0;
    virtual SystemStats getCurrentSystemStats() const = 0;
    virtual QList<ProcessInfo> getCurrentProcesses() const = 0;
};

#endif // ISYSTEMMONITOR_H
