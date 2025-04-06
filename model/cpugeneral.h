#ifndef CPUGENERAL_H
#define CPUGENERAL_H

class CpuGeneral
{
public:
    CpuGeneral();
    CpuGeneral(double util, double temp, double freq_current, double freq_max, double freq_min);

    double utilization() const;
    double temperature() const;
    double frequencyCurrent() const;
    double frequencyMax() const;
    double frequencyMin() const;

    void setUtilization(double util);
    void setTemperature(double temp);
    void setFrequencyCurrent(double freq_current);
    void setFrequencyMax(double freq_max);
    void setFrequencyMin(double freq_min);

private:
    double m_utilization;
    double m_temperature;
    double m_frequencyCurrent;
    double m_frequencyMax;
    double m_frequencyMin;
};

#endif // CPUGENERAL_H
