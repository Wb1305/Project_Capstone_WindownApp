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
    root["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QJsonObject systemStats;

    // --- Tổng giá trị CPU & RAM ---
    int cpuUtil = QRandomGenerator::global()->bounded(60, 85); // CPU tổng (%)
    double temp = 40.0 + (cpuUtil * 0.5);
    double freqPercent = 30.0 + (cpuUtil * 0.7);
    double freq = 1200.0 + (freqPercent / 100.0 * 1800.0); // 1.2GHz đến 3.0GHz

    double maxRam = 16384.0; // MB
    double maxSwap = 2048.0;
    int ramUsageMB = QRandomGenerator::global()->bounded(4000, 8000); // RAM đang dùng
    int ramPercent = static_cast<int>((ramUsageMB / maxRam) * 100);
    int swapPercent = QRandomGenerator::global()->bounded(5, 50);
    int swapUsage = (swapPercent / 100.0) * maxSwap;

    // --- General CPU ---
    QJsonObject generalCpu;
    generalCpu["CPUUtilization"] = cpuUtil;
    generalCpu["CPUTemperature"] = temp;
    generalCpu["CPUFrequency"] = freq;
    generalCpu["CPUFrequencyPercent"] = freqPercent;

    // --- Cores CPU ---
    QJsonObject coresCpu;
    for (int i = 0; i < 8; ++i) {
        int coreUtil = QRandomGenerator::global()->bounded(qMax(5, cpuUtil - 10), qMin(100, cpuUtil + 10));
        int offset = QRandomGenerator::global()->bounded(0, 201) - 100; // [-100, 100]
        double coreFreq = qMax(1000.0, freq + offset);
        double coreTemp = 40.0 + (coreUtil * 0.6);

        QJsonObject core;
        core["CPUUtilization"] = coreUtil;
        core["CPUFrequency"] = coreFreq;
        core["CPUTemperature"] = coreTemp;
        coresCpu[QString::number(i)] = core;
    }

    // --- Memory ---
    QJsonObject mem;
    mem["MaxRAM"] = maxRam;
    mem["MaxSWAP"] = maxSwap;
    mem["RAMPercent"] = ramPercent;
    mem["SWAPPercent"] = swapPercent;
    mem["RAMUsage"] = ramUsageMB;
    mem["SWAPUsage"] = swapUsage;

    systemStats["GeneralCPU"] = generalCpu;
    systemStats["coresCPU"] = coresCpu;
    systemStats["MEM"] = mem;
    root["SystemStats"] = systemStats;

    // --- Processes ---
    // Bước 1: Định nghĩa số process quá tải và tài nguyên tối thiểu của chúng
    int overloadCount = 3;
    int overloadCpuPerProcess = 10;      // mỗi tiến trình dùng 15%
    int overloadRamPerProcess = 400;     // mỗi tiến trình dùng 600MB

    int reservedCpu = overloadCount * overloadCpuPerProcess;    // 125%
    int reservedRam = overloadCount * overloadRamPerProcess;    // 3000MB

    // Giới hạn phần còn lại để chia đều cho process bình thường
    int remainingCpu = qMax(0, cpuUtil - reservedCpu);
    int remainingRam = qMax(0, ramUsageMB - reservedRam);

    // Chuẩn bị trọng số
    QVector<double> cpuWeights, ramWeights;
    double cpuSum = 0, ramSum = 0;
    for (int i = 0; i < 50 - overloadCount; ++i) {
        double wCpu = QRandomGenerator::global()->bounded(1, 100);
        double wRam = QRandomGenerator::global()->bounded(1, 100);
        cpuWeights.append(wCpu); cpuSum += wCpu;
        ramWeights.append(wRam); ramSum += wRam;
    }

    QJsonObject processes;
    double totalCpuProcess = 0;
    double totalRamProcess = 0.0;

    for (int i = 0; i < 50; ++i) {
        int cpuP = 0;
        int memMB = 0;

        if (i < overloadCount) {
            // Gán trực tiếp process quá tải
            cpuP = overloadCpuPerProcess + QRandomGenerator::global()->bounded(5, 10);  // 25~34%
            memMB = overloadRamPerProcess + QRandomGenerator::global()->bounded(100, 300);  // 600~900MB
        } else {
            int index = i - overloadCount;
            cpuP = std::round(remainingCpu * (cpuWeights[index] / cpuSum));
            memMB = std::round(remainingRam * (ramWeights[index] / ramSum));
        }

        int memPercent = static_cast<int>((memMB / maxRam) * 100);
        totalCpuProcess += cpuP;
        totalRamProcess += memMB;

        QJsonObject proc;
        proc["PID"] = 1000 + i;
        proc["User"] = "user-name";
        proc["PName"] = QString("App_%1").arg(i + 1);
        proc["PCPUUsagePercent"] = cpuP;
        proc["PMEMUsageMB"] = memMB;
        proc["PMEMUsagePercent"] = memPercent;

        processes[QString::number(1000 + i)] = proc;
    }

    int finalCpuUtil = qMin(100, static_cast<int>(totalCpuProcess));
    int finalRamUsage = qMin(static_cast<int>(maxRam), static_cast<int>(totalRamProcess));
    int finalRamPercent = static_cast<int>((finalRamUsage / maxRam) * 100);

    generalCpu["CPUUtilization"] = finalCpuUtil;
    mem["RAMUsage"] = finalRamUsage;
    mem["RAMPercent"] = finalRamPercent;
    systemStats["GeneralCPU"] = generalCpu;
    systemStats["MEM"] = mem;

    root["SystemStats"] = systemStats;
    root["ProcessesStats"] = processes;
    return QJsonDocument(root).toJson(QJsonDocument::Compact);
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

QByteArray SystemMonitor::createCommandKillProcessJson(const QString &procName)
{
    qDebug()<<"=== Kill proesses ===";
    QJsonObject obj;
    obj["type"] = "killProcess";
    obj["PNames"] = procName;

    QJsonDocument doc(obj);
    QByteArray commandJson = doc.toJson(QJsonDocument::Compact);

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
        emit systemUsageChanged(m_systemStats);
    });
}

void SystemMonitor::setOverloadDetector(OverloadDetector *detector)
{
    m_detector = detector;
    connect(m_detector, &OverloadDetector::overloadDetected, this, &SystemMonitor::onOverloadDetected, Qt::QueuedConnection);
}

void SystemMonitor::setProcessManager(ProcessManager *procManager)
{
    m_processManager = procManager;
    // connect(this, &SystemMonitor::processListReady, m_processManager, &ProcessManager::handleOverload);
    connect(m_processManager, &ProcessManager::killProcessRequested, this, &SystemMonitor::onCommandKillProcessReceived, Qt::QueuedConnection);
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

void SystemMonitor::onCommandKillProcessReceived(const QString &procName)
{
    QByteArray command = createCommandKillProcessJson(procName);
    qDebug()<<"=== Command kill process ==="<<command;
    emit commandReceived(command);
}
