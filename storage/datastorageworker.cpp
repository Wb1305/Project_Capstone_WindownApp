#include "datastorageworker.h"
#include "core/overloaddetector.h"
#include <QDateTime>
#include <QDebug>
#include <QQueue>

DataStorageWorker::DataStorageWorker(const QString &baseDir, QObject *parent)
    : QObject{parent},
    m_writerManager(baseDir)
{
    m_currentDate = QDate::currentDate();
    qDebug()<<"[DataStorageWorker] Base dir: "<<baseDir;
}

void DataStorageWorker::save(const SystemStats &systemStats, const QVector<ProcessInfo> &processes)
{
    // qDebug()<<"[DataStorageWorker] === Saving data ===";
    writeCpuCores(systemStats);
    writeProcesses(processes);
}

void DataStorageWorker::saveOverloadBuffer(const QQueue<OverloadSnapshot> &buffer)
{
    if(buffer.isEmpty()){
        qWarning() << "[DataStorageWorker] Received empty overload buffer.";
        return;
    }

    qDebug() << "[DataStorageWorker] Saving overload buffer. Count:" << buffer.size();

    for(const OverloadSnapshot &snapshot : buffer){
        // save(snapshot.m_systemStats, snapshot.m_processes);
        writeOverloadMetrics(snapshot.m_systemStats, snapshot.detectedState, snapshot.score, snapshot.balancePenatyVal);
        writeProcesses(snapshot.m_processes);
    }
}

void DataStorageWorker::writeCpuGeneral(const SystemStats &systemStats)
{
    // QString dateStr = systemStats.timestamp().date().toString("yyyy-MM-dd");
    QString dateStr = systemStats.timestamp().toString(Qt::ISODate);
    QStringList row = { dateStr };
    row += systemStats.cpuStats().general().toRow();

    CsvWriter* writer = m_writerManager.createCsvWriter("cpu_general", m_currentDate);
    QStringList headers = { "Time Stamp", "CPU(%)", "CPU Temperature(độ C)", "CPU Freq(GHz)", "CPU Freq(%)" };

    if(!writer->isFileExists()){
        writer->appendHeader(headers);
    }

    writer->appendRow(row);
}

void DataStorageWorker::writeCpuCores(const SystemStats &systemStats)
{
    // QString dateStr = systemStats.timestamp().date().toString("yyyy-MM-dd");
    QString dateStr = systemStats.timestamp().toString(Qt::ISODate);
    QStringList headers = { "Time Stamp", "CoreID", "CPU(%)", "CPU Temperature(C)", "CPU Freq (GHz)" };

    CsvWriter* writer = m_writerManager.createCsvWriter("cpu_cores", m_currentDate);
    const QVector<CpuCore>& coreList = systemStats.cpuStats().cores();

    if(!writer->isFileExists()){
        writer->appendHeader(headers);
    }

    for(const CpuCore& core : coreList){
        QStringList row = { dateStr };
        row += core.toRow();
        writer->appendRow(row);
    }
}

void DataStorageWorker::writeMemory(const SystemStats &systemStats)
{
    // QString dateStr = systemStats.timestamp().date().toString("yyyy-MM-dd");
    QString dateStr = systemStats.timestamp().toString(Qt::ISODate);
    QStringList row = { dateStr };
    row += systemStats.memStats().toRow();

    QStringList headers = {"Time Stamp", "RAM(MB)", "RAM(%)", "SWAP(MB)", "SWAP(%)", "Max RAM(MB)", "Max SWAP(MB)"};
    CsvWriter* writer = m_writerManager.createCsvWriter("memory", m_currentDate);

    if(!writer->isFileExists()){
        writer->appendHeader(headers);
    }

    writer->appendRow(row);
}

void DataStorageWorker::writeProcesses(const QVector<ProcessInfo> &processes)
{
    const QVector<ProcessInfo>& list = processes;
    QStringList headers = {"Time Stamp", "PID", "User", "PName", "CPU(%)", "Memory(MB)", "Memory(%)"};
    CsvWriter* writer = m_writerManager.createCsvWriter("processes", m_currentDate);

    if(!writer->isFileExists()){
        writer->appendHeader(headers);
    }

    for(const ProcessInfo& proc : list){
        writer->appendRow(proc.toRow());
    }
}

// lưu cpuGeneral + mem
void DataStorageWorker::writeOverloadMetrics(const SystemStats &systemStats,
                                             int stateCurr, double score, double balancePenatyVal)
{
    QString dateStr = systemStats.timestamp().toString(Qt::ISODate);
    CsvWriter* writer = m_writerManager.createCsvWriter("cpu_memory_stats", m_currentDate);
    QStringList headers = {
        "Time Stamp",
        "CPU(%)", "CPU Temperature(C)", "CPU Freq(GHz)", "CPU Freq(%)",
        "RAM(MB)", "RAM(%)", "SWAP(MB)", "SWAP(%)",
        "Max RAM(MB)", "Max SWAP(MB)", "BalancePenaty", "Score", "StateNumber", "State"
    };

    if(!writer->isFileExists()){
        writer->appendHeader(headers);
    }

    QStringList row = { dateStr };

    // Gộp dữ liệu CPU General
    row += systemStats.cpuStats().general().toRow();

    // Gộp dữ liệu Memory
    row += systemStats.memStats().toRow();

    row += QString::number(balancePenatyVal);
    row += QString::number(score);
    row += QString::number(stateCurr);

    QString stateStr;
    switch (static_cast<OverloadDetector::LoadLevel>(stateCurr)) {
    case OverloadDetector::LoadLevel::STATE_NORMAL:
        stateStr = "Normal"; break;
    case OverloadDetector::LoadLevel::STATE_WARNING:
        stateStr = "Warning"; break;
    case OverloadDetector::LoadLevel::STATE_OVERLOADED:
        stateStr = "Overloaded"; break;
    }

    row += stateStr;

    writer->appendRow(row);
}

void DataStorageWorker::writeOverloadProcessLog(const QVector<ProcessInfo> &procList,
                                                const QString &killedProc,
                                                const QVector<QPair<int, float> > &scoreMap,
                                                const QHash<QString, int> &priorityMap)
{
    qDebug()<<"[DataStorageWorker] Save Moment Overload Process Log";

    CsvWriter* writer = m_writerManager.createCsvWriter("overload_process_log", m_currentDate);

    QStringList headers = { "Time Stamp", "PID", "User", "ProcessName", "CPU(%)", "MEM(MB)", "MEM(%)", "Priority", "Score", "Is Killed" };
    if (!writer->isFileExists()) {
        writer->appendHeader(headers);
    }

    for(const ProcessInfo &proc : procList){
        QStringList row = proc.toRow();

        int procPID = proc.pid();
        QString procName = proc.name();

        float score = 0.0f;
        bool scoreFound = false;

        for (const auto &pair : scoreMap) {
            if (pair.first == procPID) {
                score = pair.second;
                scoreFound = true;
                break;
            }
        }

        if (!scoreFound) {
            qWarning() << "Warning: Process PID " << procPID << "does not have a score in scoreMap.";
        }

        int priority = priorityMap.value(procName, 2);
        QString killedFlag = (procName == killedProc) ? "Yes" : " ";

        row << QString::number(priority)
            << QString::number(score, 'f', 2)
            << killedFlag;

        writer->appendRow(row);
    }
}
