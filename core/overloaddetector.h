#ifndef OVERLOADDETECTOR_H
#define OVERLOADDETECTOR_H

// #include "model/systemstats.h"
// #include "model/processinfo.h"
#include <QObject>

class OverloadDetector : public QObject
{
    Q_OBJECT
public:
    explicit OverloadDetector(QObject *parent = nullptr);

private:
    bool checkOverload(double currentCpu, double currentRam) const;
signals:
    void overloadDetected();
    // void overloadDetectedWithProcesses(const QVector<ProcessInfo>& processList);
public slots:
    void updateUsage(double currentCpu, double currentRam);

};

#endif // OVERLOADDETECTOR_H
