#include "systemmem.h"

SystemMEM::SystemMEM() {}

SystemMEM::SystemMEM(double maxRam, double maxSwap, double ramUse, double swapUse)
    : m_maxRamSystem(maxRam), m_maxSwapSystem(maxSwap), m_ramUtilization(ramUse), m_swapUtilization(swapUse){}

double SystemMEM::maxRamSystem() const
{
    return m_maxRamSystem;
}

double SystemMEM::maxSwapSystem() const
{
    return m_maxSwapSystem;
}

double SystemMEM::ramUtilization() const
{
    return m_ramUtilization;
}

double SystemMEM::swapUtilization() const
{
    return m_swapUtilization;
}

void SystemMEM::setMaxRamSystem(double maxRam)
{
    m_maxRamSystem = maxRam;
}

void SystemMEM::setMaxSwapSystem(double maxSwap)
{
    m_maxSwapSystem = maxSwap;
}

void SystemMEM::setRamUtilization(double ramUse)
{
    m_ramUtilization = ramUse;
}

void SystemMEM::setSwapUtilization(double swapUse)
{
    m_swapUtilization = swapUse;
}

bool SystemMEM::fromJson(const QJsonObject &memObj)
{
    if (!memObj.contains("RAM") ||
        !memObj.contains("RAMPercent") ||
        !memObj.contains("SWAP") ||
        !memObj.contains("SWAPPercent"))
    {
        qWarning() << "[SystemMEM] Missing one or more required fields: RAM, RAMPercent, SWAP, SWAPPercent";
        return false;
    }
    m_maxRamSystem = memObj["RAM"].toDouble();
    m_ramUtilization = memObj["RAMPercent"].toDouble();
    m_maxSwapSystem = memObj["SWAP"].toDouble();
    m_swapUtilization = memObj["SWAPPercent"].toDouble();

    return true;
}

