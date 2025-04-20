#include "systemmonitor.h"
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QDebug>


SystemMonitor::SystemMonitor(QObject *parent)
    : QObject(parent),
    m_iviServer(new IviSocketServer(this)),
    m_dataProcessor(new DataProcessor(this)),
    m_config(new ConfigManager(this))
    /*m_detector(new OverloadDetector(this)),*/
    /*m_processManager(new ProcessManager(this))*/
{
    // connect(m_iviServer, &IviSocketServer::dataReceived, this, &SystemMonitor::onDataReceived);
    bindToDataProcessor(m_dataProcessor); // dùng để test với data fake
}

void SystemMonitor::startMonitoring()
{
    qDebug() << "[SystemMonitor] Start monitoring...";

    // 1. Connect signal-slot tại đây
    connect(m_iviServer, &IviSocketServer::dataReceived, this, &SystemMonitor::onDataReceived);
    connect(this, &SystemMonitor::commandReceived, m_iviServer, &IviSocketServer::onCommandReceived);
    // bindToDataProcessor(m_dataProcessor);

    // 2. Bắt đầu lắng nghe
    quint16 port = m_config->getValue("server/port").toUInt();
    m_iviServer->startListening(port);

}

void SystemMonitor::stopMonitoring()
{
    m_iviServer->stopListening();
    qDebug()<< "Stop monitor.";
}

SystemStats SystemMonitor::getCurrentSystemStats() const
{
    // return m_dataProcessor ? m_dataProcessor->systemStats() : SystemStats{};
    return m_systemStats;
}

QVector<ProcessInfo> SystemMonitor::getCurrentProcesses() const
{
    // return m_dataProcessor ? m_dataProcessor->processList() : QList<ProcessInfo>{};
    return m_processList;
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
    mem["MaxRAM"] = 16384,0;
    mem["MaxSWAP"] = 2048.0;
    mem["RAMPercent"] = QRandomGenerator::global()->bounded(30, 90);
    mem["SWAPPercent"] = QRandomGenerator::global()->bounded(5, 40);
    mem["RAMUsage"] = QRandomGenerator::global()->bounded(500, 2000);
    mem["SWAPUsage"] = QRandomGenerator::global()->bounded(100, 1000);
    systemStats["MEM"] = mem;

    root["SystemStats"] = systemStats;

    // === ProcessesStats ===
    QJsonObject processes;
    for (int i = 0; i < 100; ++i) {
        QJsonObject proc;
        proc["PID"] = 1000 + i;
        proc["User"] = "user-name";
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
    qDebug() << "  RAM Usage:" << mem.ramUtilization() << "MB (" << mem.ramPercent() << "%)" << " of "<<mem.maxRamSystem();
    qDebug() << "  SWAP Usage:" << mem.swapUtilization() << "MB (" << mem.swapPercent() << "%)" << " of "<<mem.maxSwapSystem();

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

void SystemMonitor::startStressTest(int numberOfTasks, int memUsagePercent, int numberOfCores, int timeout)
{
    qDebug() << "Stress Test started with params:";
    qDebug() << "Number of Tasks:" << numberOfTasks;
    qDebug() << "MEM Usage Percent:" << memUsagePercent;
    qDebug() << "Number of Cores:" << numberOfCores;
    qDebug() << "Timeout:" << timeout;

    QByteArray commandJson = createCommandForStressTestJson(numberOfTasks, memUsagePercent, numberOfCores, timeout);

    emit commandReceived(commandJson);
}

void SystemMonitor::stopStressTest()
{
    qDebug() << "Stop Stress Test";
    QByteArray commandJson = createCommandStopStressJson();

    emit commandReceived(commandJson);
}

QByteArray SystemMonitor::createCommandForStressTestJson(int numberOfTasks, int memUsagePercent, int numberOfCores, int timeout)
{
    QJsonObject jsonObject;
    jsonObject["type"] = "startStress";
    jsonObject["numberOfTaskToRun"] = numberOfTasks;
    jsonObject["MEMUsagePercent"] = memUsagePercent;
    jsonObject["numberOfCore"] = numberOfCores;
    jsonObject["timeout"] = timeout;

    QJsonDocument jsonDoc(jsonObject);
    QByteArray commandJson = jsonDoc.toJson(QJsonDocument::Compact);

    // In ra JSON để kiểm tra
    qDebug() << "Generated JSON:" << commandJson;

    return commandJson;
}

QByteArray SystemMonitor::createCommandStopStressJson()
{
    QJsonObject jsonObject;
    jsonObject["type"] = "stopStress";

    QJsonDocument jsonDoc(jsonObject);
    QByteArray commandJson = jsonDoc.toJson(QJsonDocument::Compact);

    qDebug() << "Generated JSON:" << commandJson;
    return commandJson;
}

void SystemMonitor::bindToDataProcessor(DataProcessor* processor)
{
    m_dataProcessor = processor;
    connect(processor, &DataProcessor::parseCompleted, this, [this](){

        m_systemStats = m_dataProcessor->systemStats();
        m_processList = m_dataProcessor->processList();

        qDebug() << "[SystemMonitor] emit systemUpdated";
        emit systemUpdated(m_systemStats, m_processList);
        // emit systemUpdated(m_dataProcessor->systemStats(), m_dataProcessor->processList());

        // double cpuCurrent = m_systemStats.cpuStats().general().utilization();
        // double ramCurrent = m_systemStats.memStats().ramUtilization();
        // emit systemUsageChanged(cpuCurrent , ramCurrent);

        emit systemUsageChanged(m_systemStats);
    });
}

void SystemMonitor::setOverloadDetector(OverloadDetector *detector)
{
    m_detector = detector;
    // connect(this, &SystemMonitor::systemUsageChanged, m_detector, &OverloadDetector::updateUsage);
    connect(m_detector, &OverloadDetector::overloadDetected, this, &SystemMonitor::onOverloadDetected);
}

void SystemMonitor::setProcessManager(ProcessManager *procManager)
{
    m_processManager = procManager;
    // connect(this, &SystemMonitor::processListReady, m_processManager, &ProcessManager::handleOverload);
}

void SystemMonitor::onDataReceived(const QByteArray &rawData)
{
    if(!m_dataProcessor) return;
    if(!m_dataProcessor->parseJsonData(rawData)){
        qWarning() << "[SystemMonitor] Failed to parse data";
    }
    // qDebug()<<"raw--data:"<< rawData;
    // printParsedData(m_dataProcessor->systemStats(), m_dataProcessor->processList());
}

void SystemMonitor::onOverloadDetected()
{
    emit processListReady(m_processList);
}
