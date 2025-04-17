#include "overloaddetector.h"
#include <QDebug>

OverloadDetector::OverloadDetector(QObject *parent)
    : QObject{parent}
{}

bool OverloadDetector::checkOverload(double currentCpu, double currentRam) const
{
    return true;
}

void OverloadDetector::updateUsage(double currentCpu, double currentRam)
{
    qDebug()<<"CPU: "<<currentCpu<<", Ram: "<<currentRam;
}
