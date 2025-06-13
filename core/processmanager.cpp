#include "processmanager.h"
#include "model/ProcessSelectionResult.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#define DATA_PATH_WHILELIST "../../config/WhileList.txt"
#define DATA_PATH_PRIORITY "../../config/PRIORITY.txt"

ProcessManager::ProcessManager(QObject *parent)
    : QObject{parent}
{}

ProcessSelectionResult ProcessManager::findProcessToKill(const QVector<ProcessInfo> &processesStats)
{
    ProcessSelectionResult result;

    m_nonRootProcesses.clear();
    m_validProcessNames.clear();

    filterNonRootProcesses(processesStats);
    applyWhitelistFilter();

    QHash<QString, int> priorityMap = loadPriorityConfig();
    QVector<QPair<int, float>> processScores = calculateProcessScores(priorityMap);

    auto maxScoreProcessIt = std::max_element(processScores.begin(), processScores.end(),
                                              [](const auto &a, const auto &b) {
                                                  return a.second < b.second;
                                              });

    int procPID;
    if (maxScoreProcessIt != processScores.end()) {
        procPID = maxScoreProcessIt->first;
    }

    result.procToKill = getProcessNameByPid(procPID);
    result.scoreMap = processScores;
    result.priorityMap = priorityMap;
    result.usedProcessList = m_nonRootProcesses;

    return result;
}

void ProcessManager::filterNonRootProcesses(const QVector<ProcessInfo> &processesStats)
{
    for(const ProcessInfo &proc : processesStats){
        if(proc.user() != "root"){
            QString procName = proc.name();
            m_validProcessNames.insert(procName);
            m_nonRootProcesses.append(proc);
        }
    }
}

void ProcessManager::applyWhitelistFilter()
{
    QFile whitelistFile(DATA_PATH_WHILELIST);
    if (whitelistFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&whitelistFile);
        while (!in.atEnd()) {
            QString procName = in.readLine().trimmed();
            if (!procName.isEmpty())
                m_validProcessNames.remove(procName);
        }
        whitelistFile.close();
    }
}

QHash<QString, int> ProcessManager::loadPriorityConfig()
{
    QHash<QString, int> priorities;
    QFile file(DATA_PATH_PRIORITY);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can't open priority file:" << DATA_PATH_PRIORITY;
        return priorities;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString configLine = in.readLine().trimmed();
        if (configLine.isEmpty()) continue;

        QStringList parts = configLine.split(":");
        if (parts.size() != 2) continue;

        QString processName = parts[0].trimmed();
        int priorityValue = parts[1].toInt();

        priorities[processName] = priorityValue;
    }

    file.close();
    return priorities;
}

QVector<QPair<int, float>> ProcessManager::calculateProcessScores(QHash<QString, int> &priorityMap)
{
    QVector<QPair<int, float>> processScores;
    for (ProcessInfo &proc : m_nonRootProcesses) {
        int pid = proc.pid();
        QString procName = proc.name();

        if (!m_validProcessNames.contains(procName))
            continue;

        float cpuPercent = proc.cpuUsagePercent();
        float memPercent = proc.memUsagePercent();
        int prio = priorityMap.value(procName, 2);

        float score = 0.35f * cpuPercent + 0.45f * memPercent + 0.2f * prio;
        processScores.append({pid, score});
    }

    // In ra PID và score
    for (const auto &pair : processScores) {
        qDebug() << "PID:" << pair.first << "Score:" << pair.second;
    }

    return processScores;
}


QVariantMap ProcessManager::createInfor(const QString &procName, const QDateTime &shutdownTime)
{
    QVariantMap info;
    info["procName"] = procName;
    info["shutdownTime"] = shutdownTime.toString(Qt::ISODate);
    return info;
}

void ProcessManager::informKilledProcessInfoForUI(const ProcessSelectionResult &result)
{
    qDebug()<<"[ProcessManger] Inform killed process info for UI";
    QString procName = result.procToKill;
    QDateTime shutdownTime = result.usedProcessList[0].timestamp();

    QVariantMap info = createInfor(procName, shutdownTime);

    emit killedProcessInfo(info);
}

QString ProcessManager::getProcessNameByPid(int pid)
{
    for (const ProcessInfo &proc : m_nonRootProcesses) {
        if (proc.pid() == pid) {
            return proc.name();
        }
    }
    qWarning() << "Process with PID" << pid << "not found.";
    return QString();
}

void ProcessManager::handleOverload(const QVector<ProcessInfo> &procList)
{
    qDebug()<<"[ProcessManger] Handle Overload";
    ProcessSelectionResult result = findProcessToKill(procList);

    if (!result.procToKill.isEmpty()) {
        emit killProcessRequested(result.procToKill);
    } else {
        qDebug() << "[ProcessManager] No suitable process found to kill.";
    }

    emit processKillDecisionReady(result.usedProcessList, result.procToKill, result.scoreMap, result.priorityMap);

    informKilledProcessInfoForUI(result);
}
