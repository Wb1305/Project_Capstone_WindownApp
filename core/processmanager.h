#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "IProcessManager.h"
#include "model/processinfo.h"

#include <QObject>
#include <QVector>
#include <QSet>
#include <QString>
#include <QHash>

class ProcessManager : public QObject, public IProcessManager
{
    Q_OBJECT
public:
    explicit ProcessManager(QObject *parent = nullptr);

    QString findProcessToKill(const QVector<ProcessInfo> &processesStats);

private:
    void filterNonRootProcesses(const QVector<ProcessInfo> &processesStats);
    void applyWhitelistFilter();
    QHash<QString, int> loadPriorityConfig();
    QVector<QPair<QString, float>> rankProcessesByScore(QHash<QString, int> &priorityMap);

signals:
    void killProcessRequested(const QString &processName);
public slots:
    void handleOverload(const QVector<ProcessInfo> &procList);

private slots:
private:
    // QVector<ProcessInfo> m_processStats;
    QVector<ProcessInfo> m_nonRootProcesses;
    QSet<QString> m_validProcessNames;
};

#endif // PROCESSMANAGER_H
