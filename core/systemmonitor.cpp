#include "systemmonitor.h"
#include <QRandomGenerator>
#include <QDebug>

// SystemMonitor::SystemMonitor() {}

SystemMonitor::SystemMonitor(QObject *parent) : ISystemMonitor(parent)
{

}

void SystemMonitor::startMonitoring()
{
    qDebug()<< "Start monitor.";
    generateFakeData();
}

void SystemMonitor::stopMonitoring()
{
    qDebug()<< "Stop monitor.";
}

SystemStats SystemMonitor::getCurrentSystemStats() const
{
    return m_systemStats;
}

QList<ProcessInfo> SystemMonitor::getCurrentProcesses() const
{
    return m_processList;
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
    generalCpu.setFrequencyMax(2.6);
    generalCpu.setFrequencyMin(1.6);

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
