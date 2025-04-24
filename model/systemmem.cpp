#include "systemmem.h"

SystemMEM::SystemMEM() {}

SystemMEM::SystemMEM(double maxRam, double maxSwap, double ramUse, double swapUse, double ramPercent, double swapPercent)
    : m_maxRamSystem(maxRam),
    m_maxSwapSystem(maxSwap),
    m_ramUtilization(ramUse),
    m_swapUtilization(swapUse),
    m_ramPercent(ramPercent),
    m_swapPercent(swapPercent)
{}

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

double SystemMEM::ramPercent() const
{
    return m_ramPercent;
}

double SystemMEM::swapPercent() const
{
    return m_swapPercent;
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

void SystemMEM::setRamPercent(double ramPercent)
{
    m_ramPercent = ramPercent;
}

void SystemMEM::setSwapPercent(double swapPercent)
{
    m_swapPercent = swapPercent;
}

bool SystemMEM::fromJson(const QJsonObject &memObj)
{
    if (!memObj.contains("RAMUsage") ||
        !memObj.contains("RAMPercent") ||
        !memObj.contains("SWAPUsage") ||
        !memObj.contains("SWAPPercent") ||
        !memObj.contains("MaxRAM") ||
        !memObj.contains("MaxSWAP"))
    {
        qWarning() << "[SystemMEM] Missing one or more required fields: RAM, RAMPercent, SWAP, SWAPPercent";
        return false;
    }
    m_maxRamSystem = memObj["MaxRAM"].toDouble();
    m_ramUtilization = memObj["RAMUsage"].toDouble();
    m_maxSwapSystem = memObj["MaxSWAP"].toDouble();
    m_swapUtilization = memObj["SWAPUsage"].toDouble();
    m_ramPercent = memObj["RAMPercent"].toDouble();
    m_swapPercent = memObj["SWAPPercent"].toDouble();

    return true;
}

QStringList SystemMEM::toRow() const
{
    return{
        QString::number(m_ramUtilization, 'f', 2),
        QString::number(m_ramPercent, 'f', 2),
        QString::number(m_swapUtilization, 'f', 2),
        QString::number(m_swapPercent, 'f', 2),
        QString::number(m_maxRamSystem, 'f', 2),
        QString::number(m_maxSwapSystem, 'f', 2)
    };
}
