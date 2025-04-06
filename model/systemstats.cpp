#include "systemstats.h"

SystemStats::SystemStats() {}

const SystemCPU &SystemStats::cpuStats() const
{
    return m_cpuStats;
}

const SystemMEM &SystemStats::memStats() const
{
    return m_memStats;
}

const QDateTime &SystemStats::timestamp() const
{
    return m_timestamp;
}

void SystemStats::setCpuStats(const SystemCPU &cpu)
{
    m_cpuStats = cpu;
}

void SystemStats::setMemStats(const SystemMEM &mem)
{
    m_memStats = mem;
}

void SystemStats::setTimestamp(const QDateTime &time)
{
    m_timestamp = time;
}

QStringList SystemStats::toRow() const
{
    return {
        m_timestamp.toString(Qt::ISODate),
        QString::number(m_cpuStats.general().utilization(), 'f', 2),
        QString::number(m_cpuStats.general().temperature(), 'f', 2),
        QString::number(m_memStats.ramUtilization(), 'f', 2),
        QString::number(m_memStats.swapUtilization(), 'f', 2)
    };
}
