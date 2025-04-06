#ifndef CPUCORE_H
#define CPUCORE_H

class CpuCore
{
public:
    CpuCore();

    CpuCore(int id, double util, double freq, double temp);

    int coreID() const;
    double utilization() const;
    double frequency() const;
    double temperature() const;

    void setCoreID(int id);
    void setUtilization(double util);
    void setFrequency(double freq);
    void setTemperature(double temp);

private:
    int m_coreID;
    double m_utilization;
    double m_frequency;
    double m_temperature;
};

#endif // CPUCORE_H
