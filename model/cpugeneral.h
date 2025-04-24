#ifndef CPUGENERAL_H
#define CPUGENERAL_H

#include <QStringList>
#include <QString>

class CpuGeneral
{
public:
    CpuGeneral();
    CpuGeneral(double util, double temp, double freq_current, double freq_percent);

    double utilization() const;
    double temperature() const;
    double frequencyCurrent() const;
    double frequencyPercent() const;
    // double frequencyMax() const;
    // double frequencyMin() const;

    void setUtilization(double util);
    void setTemperature(double temp);
    void setFrequencyCurrent(double freq_current);
    void setFrequencypercent(double freq_percent);

    // void setFrequencyMax(double freq_max);
    // void setFrequencyMin(double freq_min);

    QStringList toRow() const;

private:
    double m_utilization = 0;
    double m_temperature = 0;
    double m_frequencyCurrent = 0;
    double m_frequencyPercent = 0;
    // double m_frequencyMax;
    // double m_frequencyMin;
};

#endif // CPUGENERAL_H
