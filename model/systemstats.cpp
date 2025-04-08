#include "systemstats.h"
#include <qjsonobject.h>

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

bool SystemStats::fromJson(const QJsonObject &obj)
{
    if(!obj.contains("SystemStats")) return false;
    QJsonObject systemObj = obj["SystemStats"].toObject();

    if(!systemObj.contains("GeneralCPU") || !systemObj.contains("coresCPU") || !systemObj.contains("MEM"))
        return false;

    QJsonObject generalCpuObj = systemObj["GeneralCPU"].toObject();
    QJsonObject coresCpuObj = systemObj["coresCPU"].toObject();
    QJsonObject memObj = systemObj["MEM"].toObject();

    bool cpuOk = m_cpuStats.fromJson(generalCpuObj, coresCpuObj);
    bool memOk = m_memStats.fromJson(memObj);

    return cpuOk & memOk;
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
