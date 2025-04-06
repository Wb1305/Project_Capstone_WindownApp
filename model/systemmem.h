#ifndef SYSTEMMEM_H
#define SYSTEMMEM_H

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

private:
    double m_maxRamSystem;
    double m_maxSwapSystem;
    double m_ramUtilization;
    double m_swapUtilization;
};

#endif // SYSTEMMEM_H
