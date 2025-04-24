#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "core/systemmonitor.h"
#include "storage/datastorageworker.h"
#include <QObject>
#include <QThread>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = nullptr);
    ~ThreadManager();

    void setup(SystemMonitor* monitor);

private:
    void setupOverloadDetector(SystemMonitor* monitor);
    void setupProcessManager(SystemMonitor* monitor);
    void setupStorageWorkers(SystemMonitor* monitor);
    DataStorageWorker* createStorageWorkerThread(const QString& dir, QThread*& thread);

signals:

private slots:
    // void handleOverloadDetected();

private:
    QThread* m_overloadThread = nullptr;
    QThread* m_processThread = nullptr;
    QThread* m_normalStorageThread = nullptr;
    QThread* m_overloadStorageThread = nullptr;

    SystemMonitor* m_monitor = nullptr;
    OverloadDetector* m_overloadDetector = nullptr;
    ProcessManager* m_processManager = nullptr;
    DataStorageWorker* m_normalStorageWorker = nullptr;
    DataStorageWorker* m_overloadStorageWorker = nullptr;
};

#endif // THREADMANAGER_H
