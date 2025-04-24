#ifndef SYSTEMMEM_H
#define SYSTEMMEM_H

#include <QString>
#include <QStringList>
#include <QJsonObject>

class SystemMEM
{
public:
    SystemMEM();
    SystemMEM(double maxRam, double maxSwap, double ramUse, double swapUse, double ramPercent, double swapPercent);

    double maxRamSystem() const;
    double maxSwapSystem() const;
    double ramUtilization() const;
    double swapUtilization() const;
    double ramPercent() const;
    double swapPercent() const;

    void setMaxRamSystem(double maxRam);
    void setMaxSwapSystem(double maxSwap);
    void setRamUtilization(double ramUse);
    void setSwapUtilization(double swapUse);
    void setRamPercent(double ramPercent);
    void setSwapPercent(double swapPercent);

    bool fromJson(const QJsonObject& memObj);

    QStringList toRow() const;

private:
    double m_maxRamSystem = 0; //MB
    double m_maxSwapSystem = 0; // MB
    double m_ramUtilization = 0; // MB
    double m_swapUtilization = 0; // MB

    double m_ramPercent = 0;
    double m_swapPercent = 0;
};

#endif // SYSTEMMEM_H
