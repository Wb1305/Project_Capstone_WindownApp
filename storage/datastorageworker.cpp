#include "datastorageworker.h"
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
    qDebug()<<"[DataStorageWorker] === Saving data ===";
    writeCpuGeneral(systemStats);
    writeCpuCores(systemStats);
    writeMemory(systemStats);
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
        save(snapshot.m_systemStats, snapshot.m_processes);
    }
}

void DataStorageWorker::writeCpuGeneral(const SystemStats &systemStats)
{
    // QString dateStr = systemStats.timestamp().date().toString("yyyy-MM-dd");
    QString dateStr = systemStats.timestamp().toString(Qt::ISODate);
    QStringList row = { dateStr };
    row += systemStats.cpuStats().general().toRow();

    CsvWriter* writer = m_writerManager.createCsvWriter("cpu_general", m_currentDate);
    QStringList headers = { "Timestamp", "CPUUtilization(%)", "CPUTemperature", "CPUFrequency", "CPUFreqPercent" };

    if(!writer->isFileExists()){
        writer->appendHeader(headers);
    }

    writer->appendRow(row);
}

void DataStorageWorker::writeCpuCores(const SystemStats &systemStats)
{
    // QString dateStr = systemStats.timestamp().date().toString("yyyy-MM-dd");
    QString dateStr = systemStats.timestamp().toString(Qt::ISODate);
    QStringList headers = { "Timestamp", "CoreID", "CPUUtilization(%)", "CPUTemperature", "CPUFrequency" };

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

    QStringList headers = {"Timestamp", "RAMUsage(MB)", "RAMPercent", "SWAPUsage(MB)", "SWAPPercent", "MaxRAM(MB)", "MaxSWAP(MB)"};
    CsvWriter* writer = m_writerManager.createCsvWriter("memory", m_currentDate);

    if(!writer->isFileExists()){
        writer->appendHeader(headers);
    }

    writer->appendRow(row);
}

void DataStorageWorker::writeProcesses(const QVector<ProcessInfo> &processes)
{
    const QVector<ProcessInfo>& list = processes;
    CsvWriter* writer = m_writerManager.createCsvWriter("processes", m_currentDate);
    QStringList headers = {"Timestamp", "PID", "User", "PName", "CPU(%)", "Memory(MB)", "Memory(%)"};

    if(!writer->isFileExists()){
        writer->appendHeader(headers);
    }

    for(const ProcessInfo& proc : list){
        writer->appendRow(proc.toRow());
    }
}
