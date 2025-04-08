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
    int m_coreID = -1;
    double m_utilization = 0;
    double m_frequency = 0;
    double m_temperature = 0;
};

#endif // CPUCORE_H
