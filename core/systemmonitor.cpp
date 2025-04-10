#include "systemmonitor.h"
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QDebug>

// SystemMonitor::SystemMonitor() {}

// SystemMonitor::SystemMonitor(QObject* parent)
//     : ISystemMonitor(parent),
//     m_config(new ConfigManager),
//     m_tcp(new TcpServer),
//     m_processor(new DataProcessor),
//     m_detector(new OverloadDetector)

SystemMonitor::SystemMonitor(QObject *parent)
    : QObject(parent),
    m_iviServer(new IviSocketServer(this)),
    m_dataProcessor(new DataProcessor(this))
{
    connect(m_iviServer, &IviSocketServer::dataReceived, this, &SystemMonitor::onDataReceived);
}

void SystemMonitor::startMonitoring()
{
    qDebug()<< "Start monitor.";
    m_iviServer->startListening(8000);
    // generateFakeData();
}

void SystemMonitor::stopMonitoring()
{
    m_iviServer->stopListening();
    qDebug()<< "Stop monitor.";
}

// SystemStats SystemMonitor::getCurrentSystemStats() const
// {
//     return m_systemStats;
// }

// QList<ProcessInfo> SystemMonitor::getCurrentProcesses() const
// {
//     return m_processList;
// }

SystemStats SystemMonitor::getCurrentSystemStats() const
{
    return m_dataProcessor ? m_dataProcessor->systemStats() : SystemStats{};
}

QVector<ProcessInfo> SystemMonitor::getCurrentProcesses() const
{
    return m_dataProcessor ? m_dataProcessor->processList() : QList<ProcessInfo>{};
}

//generate fake data
QByteArray SystemMonitor::generateFakeData()
{
    QJsonObject root;

    // Timestamp
    root["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // === SystemStats ===
    QJsonObject systemStats;

    // GeneralCPU
    QJsonObject generalCpu;
    generalCpu["CPUUtilization"] = QRandomGenerator::global()->bounded(10, 80);
    generalCpu["CPUTemperature"] = QRandomGenerator::global()->bounded(45, 75);
    generalCpu["CPUFrequency"] = 2200.0;
    generalCpu["CPUFrequencyPercent"] = 65.0;
    systemStats["GeneralCPU"] = generalCpu;

    // coresCPU
    QJsonObject coresCpu;
    for (int i = 0; i < 8; ++i) {
        QJsonObject core;
        core["CPUUtilization"] = QRandomGenerator::global()->bounded(5, 70);
        core["CPUFrequency"] = 2200.0 + QRandomGenerator::global()->bounded(100);
        core["CPUTemperature"] = QRandomGenerator::global()->bounded(40, 75);
        coresCpu[QString::number(i)] = core;
    }
    systemStats["coresCPU"] = coresCpu;

    // MEM
    QJsonObject mem;
    mem["RAM"] = 16000.0;
    mem["SWAP"] = 2000.0;
    mem["RAMPercent"] = QRandomGenerator::global()->bounded(30, 90);
    mem["SWAPPercent"] = QRandomGenerator::global()->bounded(5, 40);
    systemStats["MEM"] = mem;

    root["SystemStats"] = systemStats;

    // === ProcessesStats ===
    QJsonObject processes;
    for (int i = 0; i < 10; ++i) {
        QJsonObject proc;
        proc["PID"] = 1000 + i;
        proc["User"] = "user";
        proc["PName"] = QString("App_%1").arg(i+1);
        proc["PCPUUsagePercent"] = QRandomGenerator::global()->bounded(0, 30);
        proc["PMEMUsageMB"] = QRandomGenerator::global()->bounded(50, 500);
        proc["PMEMUsagePercent"] = QRandomGenerator::global()->bounded(1, 25);

        processes[QString::number(1000 + i)] = proc;
    }
    root["ProcessesStats"] = processes;

    // Convert to QByteArray
    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Compact);
}

void SystemMonitor::printParsedData(const SystemStats &systemStats, const QVector<ProcessInfo> &processes)
{
    qDebug() << "\n[SystemMonitor] Parsed System Stats:";
    qDebug() << "Timestamp:" << systemStats.timestamp().toString("yyyy-MM-dd HH:mm:ss");

    // CPU
    const SystemCPU& cpu = systemStats.cpuStats();
    qDebug() << "General CPU:";
    qDebug() << "  Utilization:" << cpu.general().utilization();
    qDebug() << "  Temperature:" << cpu.general().temperature();
    qDebug() << "  Frequency Current:" << cpu.general().frequencyCurrent();

    qDebug() << "CPU Cores:";
    for (const CpuCore& core : cpu.cores()) {
        qDebug() << "  Core" << core.coreID()
        << "Usage:" << core.utilization()
        << "Freq:" << core.frequency()
        << "Temp:" << core.temperature();
    }

    // MEM
    const SystemMEM& mem = systemStats.memStats();
    qDebug() << "MEM:";
    qDebug() << "  RAM:" << mem.maxRamSystem() << "MB (" << mem.ramUtilization() << "%)";
    qDebug() << "  SWAP:" << mem.maxSwapSystem() << "MB (" << mem.swapUtilization() << "%)";

    // Processes
    qDebug() << "\nProcesses:";
    for (const ProcessInfo& proc : processes) {
        qDebug() << "Name:" << proc.name()
        << "User:" <<proc.user()
        << "PID:" << proc.pid()
        << "CPU %:" << proc.cpuUsagePercent()
        << "MEM MB:" << proc.memUsageMB()
        << "MEM %:" << proc.memUsagePercent();
    }
}

void SystemMonitor::feedFakeData(const QByteArray &fakeData)
{
    onDataReceived(fakeData);
}

void SystemMonitor::setDataProcessor(DataProcessor* processor)
{
    m_dataProcessor = processor;
    connect(processor, &DataProcessor::dataUpdated, this, [this](){
        emit systemUpdated(m_dataProcessor->systemStats(), m_dataProcessor->processList());
    });
}

void SystemMonitor::onDataReceived(const QByteArray &rawData)
{
    // qDebug() << "[SystemMonitor] Raw data from socket:";
    // qDebug().noquote() << QString::fromUtf8(rawData);

    if(!m_dataProcessor) return;
    if(!m_dataProcessor->parseJsonData(rawData)){
        qWarning() << "[SystemMonitor] Failed to parse data";
    }

    // printParsedData(m_dataProcessor->systemStats(), m_dataProcessor->processList());
}
