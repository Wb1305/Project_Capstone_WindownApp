#include "systemcpu.h"

SystemCPU::SystemCPU() {}

const QVector<CpuCore> &SystemCPU::cores() const
{
    return m_cores;
}

const CpuGeneral &SystemCPU::general() const
{
    return m_general;
}

void SystemCPU::setCores(const QList<CpuCore> &cores)
{
    m_cores = cores;
}

void SystemCPU::setGeneral(const CpuGeneral &general)
{
    m_general = general;
}

bool SystemCPU::fromJson(const QJsonObject &generalCpuObj, const QJsonObject &coresObj)
{
    if (!generalCpuObj.contains("CPUUtilization")) return false;
    if (!generalCpuObj.contains("CPUTemperature")) return false;
    if (!generalCpuObj.contains("CPUFrequency")) return false;
    if (!generalCpuObj.contains("CPUFrequencyPercent")) return false;

    m_general.setUtilization(generalCpuObj["CPUUtilization"].toDouble());
    m_general.setTemperature(generalCpuObj["CPUTemperature"].toDouble());
    m_general.setFrequencyCurrent(generalCpuObj["CPUFrequency"].toDouble());
    m_general.setFrequencypercent(generalCpuObj["CPUFrequencyPercent"].toDouble());

    if(coresObj.isEmpty()){
        qWarning()<<"[SystemCPU] Warning: coresCPU is empty!";
        return false;
    }
    m_cores.clear();
    for (const QString& key : coresObj.keys()) {
        QJsonObject cObj = coresObj[key].toObject();
        if (!cObj.contains("CPUUtilization") || !cObj.contains("CPUTemperature") || !cObj.contains("CPUFrequency"))
            continue;

        CpuCore core;
        core.setCoreID(key.toInt());
        core.setUtilization(cObj["CPUUtilization"].toDouble());
        core.setTemperature(cObj["CPUTemperature"].toDouble());
        core.setFrequency(cObj["CPUFrequency"].toDouble());
        m_cores.append(core);
    }
    return true;
}
