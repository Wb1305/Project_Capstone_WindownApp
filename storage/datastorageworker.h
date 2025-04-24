#ifndef DATASTORAGEWORKER_H
#define DATASTORAGEWORKER_H

#include "model/OverloadSnapshot.h"
#include "model/processinfo.h"
#include "model/systemstats.h"
#include "storage/csvwritermanager.h"
#include <QObject>

class DataStorageWorker : public QObject
{
    Q_OBJECT
public:
    explicit DataStorageWorker(const QString& baseDir, QObject *parent = nullptr);

public slots:
    void save(const SystemStats& systemStats, const QVector<ProcessInfo> &processes);
    void saveOverloadBuffer(const QQueue<OverloadSnapshot>& buffer);

private:
    void writeCpuGeneral(const SystemStats& systemStats);
    void writeCpuCores(const SystemStats& systemStats);
    void writeMemory(const SystemStats& systemStats);
    void writeProcesses(const QVector<ProcessInfo>& processes);

private:
    CsvWriterManager m_writerManager;
    QDate m_currentDate;

signals:
};

#endif // DATASTORAGEWORKER_H
