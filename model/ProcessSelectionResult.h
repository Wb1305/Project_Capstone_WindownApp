#ifndef PROCESSSELECTIONRESULT_H
#define PROCESSSELECTIONRESULT_H

#include "model/processinfo.h"
#include <QString>

struct ProcessSelectionResult{
    QString procToKill;
    // QHash<QString, float> scoreMap;
    QVector<QPair<int, float>> scoreMap;
    QHash<QString, int> priorityMap;
    QVector<ProcessInfo> usedProcessList;
};

#endif // PROCESSSELECTIONRESULT_H
