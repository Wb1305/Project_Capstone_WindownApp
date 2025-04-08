#ifndef SYSTEMMEM_H
#define SYSTEMMEM_H

#include <QJsonObject>

class SystemMEM
{
public:
    SystemMEM();
    SystemMEM(double maxRam, double maxSwap, double ramUse, double swapUse);

    double maxRamSystem() const;
    double maxSwapSystem() const;
    double ramUtilization() const;
    double swapUtilization() const;

    void setMaxRamSystem(double maxRam);
    void setMaxSwapSystem(double maxSwap);
    void setRamUtilization(double ramUse);
    void setSwapUtilization(double swapUse);

    bool fromJson(const QJsonObject& memObj);

private:
    double m_maxRamSystem = 0;
    double m_maxSwapSystem = 0;
    double m_ramUtilization = 0;
    double m_swapUtilization = 0;
};

#endif // SYSTEMMEM_H
