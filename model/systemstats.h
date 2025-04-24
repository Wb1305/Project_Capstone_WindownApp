#ifndef SYSTEMSTATS_H
#define SYSTEMSTATS_H

#include <QDateTime>
#include "model/systemcpu.h"
#include "model/systemmem.h"
#include <QJsonObject>
#include <QString>

class SystemStats
{
public:
    SystemStats();

    const SystemCPU& cpuStats() const;
    const SystemMEM& memStats() const;
    const QDateTime& timestamp() const;

    void setCpuStats(const SystemCPU& cpu);
    void setMemStats(const SystemMEM& mem);
    void setTimestamp(const QDateTime& time);

    bool fromJson(const QJsonObject& obj);
    QStringList toRow() const;

private:
    SystemCPU m_cpuStats;
    SystemMEM m_memStats;
    QDateTime m_timestamp;
};

#endif // SYSTEMSTATS_H
