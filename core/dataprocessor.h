#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "model/processinfo.h"
#include "model/systemstats.h"
#include <QObject>

class DataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = nullptr);

    const SystemStats& systemStats() const;
    const QVector<ProcessInfo>& processList() const;
    QDateTime timestamp() const;

    bool parseJsonData(const QByteArray& jsonData);
signals:
    // void dataUpdated();
    void parseCompleted();
private:
    SystemStats m_systemStats;
    QVector<ProcessInfo> m_processes;
    QDateTime m_timestamp;
};

#endif // DATAPROCESSOR_H
