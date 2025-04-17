#include "processmanager.h"
#include <QDebug>

ProcessManager::ProcessManager(QObject *parent)
    : QObject{parent}
{}

void ProcessManager::handleOverload(const QVector<ProcessInfo> &procList)
{
    qDebug()<<"Handle Overload";
}
