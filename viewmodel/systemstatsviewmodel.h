#ifndef SYSTEMSTATSVIEWMODEL_H
#define SYSTEMSTATSVIEWMODEL_H

#include "core/systemmonitor.h"
#include "model/systemstats.h"
#include "viewmodel/cpustatsviewmodel.h"
#include "viewmodel/memstatsviewmodel.h"
#include <QObject>

class SystemStatsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CpuStatsViewModel* cpu READ cpu CONSTANT)
    Q_PROPERTY(MemStatsViewModel* mem READ mem CONSTANT)
public:
    explicit SystemStatsViewModel(QObject *parent = nullptr);

    CpuStatsViewModel* cpu() const;
    MemStatsViewModel* mem() const;

    void updateFromStats(const SystemStats& systemStats);
    void bindToMonitor(SystemMonitor* monitor);
private:
    CpuStatsViewModel* m_cpu;
    MemStatsViewModel* m_mem;
};

#endif // SYSTEMSTATSVIEWMODEL_H
