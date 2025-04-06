#include "cpugeneral.h"

CpuGeneral::CpuGeneral() {}

CpuGeneral::CpuGeneral(double util, double temp, double freq_current, double freq_max, double freq_min)
    : m_utilization(util), m_temperature(temp), m_frequencyCurrent(freq_current), m_frequencyMax(freq_max), m_frequencyMin(freq_min){}

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

double CpuGeneral::frequencyMax() const
{
    return m_frequencyMax;
}

double CpuGeneral::frequencyMin() const
{
    return m_frequencyMin;
}

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

void CpuGeneral::setFrequencyMax(double freq_max)
{
    m_frequencyMax = freq_max;
}

void CpuGeneral::setFrequencyMin(double freq_min)
{
    m_frequencyMin = freq_min;
}





