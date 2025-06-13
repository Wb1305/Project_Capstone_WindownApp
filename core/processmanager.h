#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "IProcessManager.h"
#include "model/ProcessSelectionResult.h"
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

    // QString findProcessToKill(const QVector<ProcessInfo> &processesStats);
    ProcessSelectionResult findProcessToKill(const QVector<ProcessInfo> &processesStats);

private:
    void filterNonRootProcesses(const QVector<ProcessInfo> &processesStats);
    void applyWhitelistFilter();
    QHash<QString, int> loadPriorityConfig();
    // QVector<QPair<QString, float>> rankProcessesByScore(QHash<QString, int> &priorityMap);
    QVector<QPair<int, float>> calculateProcessScores(QHash<QString, int> &priorityMap);
    QVariantMap createInfor(const QString &procName, const QDateTime &shutdownTime);
    void informKilledProcessInfoForUI(const ProcessSelectionResult &result);
    QString getProcessNameByPid(int pid);

signals:
    void killProcessRequested(const QString &processName);
    void processKillDecisionReady(const QVector<ProcessInfo> &procList,
                                  const QString &killedProc,
                                  const QVector<QPair<int, float>> &scoreMap,
                                  const QHash<QString, int> &priorityMap);
    void killedProcessInfo(const QVariantMap &info);

public slots:
    void handleOverload(const QVector<ProcessInfo> &procList);

private slots:
private:
    // QVector<ProcessInfo> m_processStats;
    QVector<ProcessInfo> m_nonRootProcesses;
    QSet<QString> m_validProcessNames;
};

#endif // PROCESSMANAGER_H
