#include "systemcpu.h"

SystemCPU::SystemCPU() {}

const QList<CpuCore> &SystemCPU::cores() const
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
