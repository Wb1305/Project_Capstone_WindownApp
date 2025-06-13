#ifndef OVERLOADSNAPSHOT_H
#define OVERLOADSNAPSHOT_H

#include "model/processinfo.h"
#include "model/systemstats.h"
#include <QDateTime>
#include <QVector>


struct OverloadSnapshot {
    QDateTime m_timestamp;
    SystemStats m_systemStats;
    QVector<ProcessInfo> m_processes;
    int detectedState;
    double score;
    double balancePenatyVal;
};

#endif // OVERLOADSNAPSHOT_H
