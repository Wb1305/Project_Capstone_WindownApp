#include "systemstatsviewmodel.h"

SystemStatsViewModel::SystemStatsViewModel(QObject *parent)
    : QObject{parent},
    m_cpu(new CpuStatsViewModel(this)),
    m_mem(new MemStatsViewModel(this))
{}

CpuStatsViewModel *SystemStatsViewModel::cpu() const
{
    return m_cpu;
}

MemStatsViewModel *SystemStatsViewModel::mem() const
{
    return m_mem;
}

void SystemStatsViewModel::updateFromStats(const SystemStats &systemStats)
{
    qDebug() << "[SystemStatsViewModel] Đã nhận được dữ liệu mới:" << systemStats.timestamp();
    m_cpu->updateFromCpu(systemStats.cpuStats());
    m_mem->updateFromMem(systemStats.memStats());
}

void SystemStatsViewModel::bindToMonitor(SystemMonitor *monitor)
{
    if (!monitor) return;
    connect(monitor, &SystemMonitor::systemUpdated,
            this, [this](const SystemStats& systemStats, const QVector<ProcessInfo>&){
                updateFromStats(systemStats);
            });
}
