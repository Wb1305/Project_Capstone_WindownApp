#ifndef SYSTEMCPU_H
#define SYSTEMCPU_H

#include <QList>
#include "model/cpucore.h"
#include "model/cpugeneral.h"

class SystemCPU
{
public:
    SystemCPU();

    const QList<CpuCore>& cores() const;
    const CpuGeneral& general() const;
    void setCores(const QList<CpuCore>& cores);
    void setGeneral(const CpuGeneral& general);

private:
    CpuGeneral m_general;
    QList<CpuCore> m_cores;
};

#endif // SYSTEMCPU_H
