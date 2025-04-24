#include "cpugeneral.h"

CpuGeneral::CpuGeneral() {}

CpuGeneral::CpuGeneral(double util, double temp, double freq_current, double freq_percent)
    : m_utilization(util),
    m_temperature(temp),
    m_frequencyCurrent(freq_current),
    m_frequencyPercent(freq_percent)
{}

double CpuGeneral::utilization() const
{
    return m_utilization;
}

double CpuGeneral::temperature() const
{
    return m_temperature;
}

double CpuGeneral::frequencyCurrent() const
{
    return m_frequencyCurrent;
}

double CpuGeneral::frequencyPercent() const
{
    return m_frequencyPercent;
}

// double CpuGeneral::frequencyMax() const
// {
//     return m_frequencyMax;
// }

// double CpuGeneral::frequencyMin() const
// {
//     return m_frequencyMin;
// }

void CpuGeneral::setUtilization(double util)
{
    m_utilization = util;
}

void CpuGeneral::setTemperature(double temp)
{
    m_temperature = temp;
}

void CpuGeneral::setFrequencyCurrent(double freq_current)
{
    m_frequencyCurrent = freq_current;
}

void CpuGeneral::setFrequencypercent(double freq_percent)
{
    m_frequencyPercent = freq_percent;
}

QStringList CpuGeneral::toRow() const
{
    return{
        QString::number(m_utilization, 'f', 2),
        QString::number(m_temperature, 'f', 2),
        QString::number(m_frequencyCurrent, 'f', 2),
        QString::number(m_frequencyPercent, 'f', 2)
    };
}

// void CpuGeneral::setFrequencyMax(double freq_max)
// {
//     m_frequencyMax = freq_max;
// }

// void CpuGeneral::setFrequencyMin(double freq_min)
// {
//     m_frequencyMin = freq_min;
// }





