#ifndef SYSTEMCPU_H
#define SYSTEMCPU_H

// #include <QList>
#include <QVector>
#include "model/cpucore.h"
#include "model/cpugeneral.h"
#include <QJsonObject>

class SystemCPU
{
public:
    SystemCPU();

    const QVector<CpuCore>& cores() const;
    const CpuGeneral& general() const;
    void setCores(const QVector<CpuCore> &cores);
    void setGeneral(const CpuGeneral& general);

    bool fromJson(const QJsonObject& generalCpuObj, const QJsonObject& coresObj);

private:
    CpuGeneral m_general;
    QVector<CpuCore> m_cores;
};

#endif // SYSTEMCPU_H
