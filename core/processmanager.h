#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "IProcessManager.h"
#include "model/processinfo.h"

#include <QObject>
#include <QVector>

class ProcessManager : public QObject, public IProcessManager
{
    Q_OBJECT
public:
    explicit ProcessManager(QObject *parent = nullptr);

signals:

public slots:
    void handleOverload(const QVector<ProcessInfo> &procList);

private slots:
private:
    QVector<ProcessInfo> m_processStats;
};

#endif // PROCESSMANAGER_H
