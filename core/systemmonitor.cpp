#include "systemmonitor.h"
#include <QRandomGenerator>
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

//fake data
void SystemMonitor::generateFakeData()
{
    SystemStats systemStats;
    systemStats.setTimestamp(QDateTime::currentDateTime());

    // Tạo dữ liệu CPU
    CpuGeneral generalCpu;
    generalCpu.setUtilization(QRandomGenerator::global()->bounded(5,60));
    generalCpu.setTemperature(QRandomGenerator::global()->bounded(40,75));
    generalCpu.setFrequencyCurrent(2.2);
    generalCpu.setFrequencypercent(2.6);


    QList<CpuCore> coreList;
    for (int i = 0; i < 8; ++i) {
        CpuCore core(i+1,
                     QRandomGenerator::global()->bounded(1, 60),
                     2.0,
                     QRandomGenerator::global()->bounded(40, 70));
        coreList.append(core);
    }

    SystemCPU cpu;
    cpu.setGeneral(generalCpu);
    cpu.setCores(coreList);
    systemStats.setCpuStats(cpu);

    // Tạo dữ liệu RAM
    SystemMEM mem(16.0, 2.0,
                  QRandomGenerator::global()->bounded(20, 70),
                  QRandomGenerator::global()->bounded(0, 20));
    systemStats.setMemStats(mem);

    // Tạo danh sách tiến trình giả
    QList<ProcessInfo> processes;
    for (int i = 0; i < 10; ++i) {
        ProcessInfo proc(
            1000 + i,
            QString("App%1").arg(i),
            "user",
            QDateTime::currentDateTime(),
            QRandomGenerator::global()->bounded(0, 30),
            QRandomGenerator::global()->bounded(0, 40),
            QRandomGenerator::global()->bounded(0, 300)
            );
        processes.append(proc);
    }

    // Lưu và phát signal
    m_systemStats = systemStats;
    m_processList = processes;
    // emit systemUpdated(stats, processes);

    // In SystemStats
    qDebug() << "===== Fake System Stats =====";
    qDebug() << "Time:" << systemStats.timestamp().toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "CPU Utilization:" << systemStats.cpuStats().general().utilization() << "%";
    qDebug() << "CPU Temp:" << systemStats.cpuStats().general().temperature() << "°C";
    qDebug() << "RAM Used:" << systemStats.memStats().maxRamSystem() << "%";
    qDebug() << "Swap Used:" << systemStats.memStats().maxRamSystem() << "GB";

    // In danh sách tiến trình
    qDebug() << "----- Process List -----";
    for (const auto& proc : processes) {
        qDebug().noquote() << QString("%1 (PID: %2) | User: %3 | CPU: %4% | MEM: %5%")
        .arg(proc.name())
            .arg(proc.pid())
            .arg(proc.user())
            .arg(proc.cpuUsagePercent(), 0, 'f', 1)
            .arg(proc.memUsagePercent(), 0, 'f', 1);
    }
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

    printParsedData(m_dataProcessor->systemStats(), m_dataProcessor->processList());
}
