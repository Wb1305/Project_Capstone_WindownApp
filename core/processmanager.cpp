#include "processmanager.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#define DATA_PATH_WHILELIST "../../config/WhileList.txt"
#define DATA_PATH_PRIORITY "../../config/PRIORITY.txt"

ProcessManager::ProcessManager(QObject *parent)
    : QObject{parent}
{}

QString ProcessManager::findProcessToKill(const QVector<ProcessInfo> &processesStats)
{
    filterNonRootProcesses(processesStats);
    applyWhitelistFilter();

    QHash<QString, int> priorityMap = loadPriorityConfig();
    QVector<QPair<QString, float>> ranked = rankProcessesByScore(priorityMap);

    // std::sort(ranked.begin(), ranked.end(),
    //           [](const auto &a, const auto &b) {
    //               return a.second > b.second;
    //           });
    // if(ranked.isEmpty()) return QString();
    // QString procNeedKill = ranked[0].first;

    auto maxScoreProcessIt = std::max_element(ranked.begin(), ranked.end(),
              [](const auto &a, const auto &b) {
                  return a.second < b.second;
              });

    if(maxScoreProcessIt == ranked.end()) return QString();
    QString procNeedKill = maxScoreProcessIt->first;

    qDebug() << "[ProcessManager] Process selected to terminate:" << procNeedKill;

    return procNeedKill;
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

QVector<QPair<QString, float> > ProcessManager::rankProcessesByScore(QHash<QString, int> &priorityMap)
{
    QVector<QPair<QString, float>> ranked;
    for (ProcessInfo &proc : m_nonRootProcesses) {
        QString procName = proc.name();
        if (!m_validProcessNames.contains(procName))
            continue;

        float cpuPercent = proc.cpuUsagePercent();
        float memPercent = proc.memUsagePercent();
        int prio = priorityMap.value(procName, 2);

        float score = 0.4f * cpuPercent + 0.4f * memPercent + 0.2f * prio;
        ranked.append({procName, score});
    }
    return ranked;
}

void ProcessManager::handleOverload(const QVector<ProcessInfo> &procList)
{
    qDebug()<<"[Thread - ProcessManger] Handle Overload";
    QString procToKill = findProcessToKill(procList);

    if (!procToKill.isEmpty()) {
        emit killProcessRequested(procToKill);
    } else {
        qDebug() << "[ProcessManager] No suitable process found to kill.";
    }
}
