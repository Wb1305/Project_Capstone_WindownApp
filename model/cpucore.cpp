#include "cpucore.h"

CpuCore::CpuCore() {}

CpuCore::CpuCore(int id, double util, double freq, double temp)
    : m_coreID(id), m_utilization(util), m_frequency(freq), m_temperature(temp) {}

int CpuCore::coreID() const
{
    return m_coreID;
}

double CpuCore::utilization() const
{
    return m_utilization;
}

double CpuCore::frequency() const
{
    return m_frequency;
}

double CpuCore::temperature() const
{
    return m_temperature;
}

void CpuCore::setCoreID(int id)
{
    m_coreID = id;
}

void CpuCore::setUtilization(double util)
{
    m_utilization = util;
}

void CpuCore::setFrequency(double freq)
{
    m_frequency = freq;
}

void CpuCore::setTemperature(double temp)
{
    m_temperature = temp;
}
