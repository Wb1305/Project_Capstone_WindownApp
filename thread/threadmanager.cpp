#include "threadmanager.h"
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDate>

ThreadManager::ThreadManager(QObject *parent)
    : QObject{parent}
{}

ThreadManager::~ThreadManager()
{

    for (QThread* thread : {m_overloadThread, m_processThread, m_normalStorageThread, m_overloadStorageThread})
    {
        if (thread) {
            thread->quit();
            thread->wait();
            delete thread;
        }
    }
}

void ThreadManager::setup(SystemMonitor *monitor)
{
    m_monitor = monitor;
    setupOverloadDetector(monitor);
    setupProcessManager(monitor);
    setupStorageWorkers(monitor);
}

void ThreadManager::setupOverloadDetector(SystemMonitor *monitor)
{
    qDebug() << "[ThreadManager] setup OverloadDetector.";

    // Khởi tạo thread nếu chưa có
    if (!m_overloadThread) {
        m_overloadThread = new QThread(this);
    }

    m_overloadDetector = new OverloadDetector();
    m_overloadDetector->moveToThread(m_overloadThread);

    connect(m_overloadThread, &QThread::finished, m_overloadDetector, &QObject::deleteLater);
    connect(monitor, &SystemMonitor::systemUpdated, m_overloadDetector, &OverloadDetector::onSystemDataReceived, Qt::QueuedConnection);
    // connect(m_overloadDetector, &OverloadDetector::overloadDetected, this, &ThreadManager::handleOverloadDetected);

    monitor->setOverloadDetector(m_overloadDetector);
    m_overloadThread->start();
}

void ThreadManager::setupProcessManager(SystemMonitor *monitor)
{
    qDebug() << "[ThreadManager] setup ProcessManager.";

    if (!m_processThread) {
        m_processThread = new QThread(this);
    }

    m_processManager = new ProcessManager();
    m_processManager->moveToThread(m_processThread);

    connect(m_processThread, &QThread::finished, m_processManager, &QObject::deleteLater);
    connect(monitor, &SystemMonitor::processListReady, m_processManager, &ProcessManager::handleOverload,
            Qt::QueuedConnection);

    monitor->setProcessManager(m_processManager);
    m_processThread->start();
}

void ThreadManager::setupStorageWorkers(SystemMonitor *monitor)
{
    qDebug() << "[ThreadManager] setup DataStorageWorker threads.";

    // Base log path trên Desktop
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString baseLogPath = QDir(desktopPath).filePath("IVISystemLogs/logsCsv");
    QString dayStr = QDate::currentDate().toString("yyyy-MM-dd");

    // [1] Ghi dữ liệu thường xuyên
    QString normalLogDir = QDir(baseLogPath).filePath("normal/" + dayStr);
    m_normalStorageWorker = createStorageWorkerThread(normalLogDir, m_normalStorageThread);
    connect(monitor, &SystemMonitor::systemUpdated,
            m_normalStorageWorker, &DataStorageWorker::save,
            Qt::QueuedConnection);

    // [2] Ghi dữ liệu quá tải
    QString overloadLogDir = QDir(baseLogPath).filePath("overload/" + dayStr);
    m_overloadStorageWorker = createStorageWorkerThread(overloadLogDir, m_overloadStorageThread);
    connect(m_overloadDetector, &OverloadDetector::overloadDetectedWithBuffer,
            m_overloadStorageWorker, &DataStorageWorker::saveOverloadBuffer,
            Qt::QueuedConnection);
}

DataStorageWorker *ThreadManager::createStorageWorkerThread(const QString &dir, QThread *&thread)
{
    QDir().mkpath(dir);

    thread = new QThread(this);
    auto* worker = new DataStorageWorker(dir);
    worker->moveToThread(thread);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    thread->start();

    return worker;
}

// void ThreadManager::handleOverloadDetected()
// {
//     qDebug() << "[ThreadManager] Overload detected.";
//     if(m_monitor && m_processManager){
//         QVector<ProcessInfo> processes = m_monitor->getCurrentProcesses();
//         QMetaObject::invokeMethod(m_processManager, [=](){
//             m_processManager->handleOverload(processes);
//         }, Qt::QueuedConnection);
//     }
// }

