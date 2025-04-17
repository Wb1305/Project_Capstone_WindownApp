#include "threadmanager.h"
#include <QDebug>

ThreadManager::ThreadManager(QObject *parent)
    : QObject{parent}
{}

ThreadManager::~ThreadManager()
{
    m_overloadThread->quit();
    m_overloadThread->wait();

    m_processThread->quit();
    m_processThread->wait();
}

void ThreadManager::setup(SystemMonitor *monitor)
{
    m_monitor = monitor;
    setupOverloadDetector(monitor);
    setupProcessManager(monitor);
}

void ThreadManager::setupOverloadDetector(SystemMonitor *monitor)
{
    qDebug() << "[ThreadManager] setup OverloadDetector.";
    m_overloadDetector = new OverloadDetector();
    m_overloadDetector->moveToThread(m_overloadThread);

    connect(m_overloadThread, &QThread::finished, m_overloadDetector, &QObject::deleteLater);
    connect(monitor, &SystemMonitor::systemUsageChanged, m_overloadDetector, &OverloadDetector::updateUsage);
    connect(m_overloadDetector, &OverloadDetector::overloadDetected, this, &ThreadManager::handleOverloadDetected);

    // Emit signal when overload detected and let process manager handle it
    // connect(m_overloadDetector, &OverloadDetector::overloadDetected,
    //         m_processManager, &ProcessManager::handleOverload, Qt::QueuedConnection);

    monitor->setOverloadDetector(m_overloadDetector);
    m_overloadThread->start();
}

void ThreadManager::setupProcessManager(SystemMonitor *monitor)
{
    qDebug() << "[ThreadManager] setup ProcessManager.";
    m_processManager = new ProcessManager();
    m_processManager->moveToThread(m_processThread);

    connect(m_processThread, &QThread::finished, m_processManager, &QObject::deleteLater);
    connect(monitor, &SystemMonitor::processListReady, m_processManager, &ProcessManager::handleOverload,
            Qt::QueuedConnection);

    monitor->setProcessManager(m_processManager);
    m_processThread->start();
}

void ThreadManager::handleOverloadDetected()
{
    qDebug() << "[ThreadManager] Overload detected.";
    // if(m_monitor && m_processManager){
    //     QVector<ProcessInfo> processes = m_monitor->getCurrentProcesses();
    //     QMetaObject::invokeMethod(m_processManager, [=](){
    //         m_processManager->handleOverload(processes);
    //     }, Qt::QueuedConnection);
    // }
}
