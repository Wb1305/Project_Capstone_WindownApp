#ifndef CPUSTATSVIEWMODEL_H
#define CPUSTATSVIEWMODEL_H

#include "model/systemcpu.h"
#include <QObject>
#include <QAbstractSeries>

class CpuStatsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QAbstractSeries*> series READ series NOTIFY seriesChanged);

public:
    explicit CpuStatsViewModel(QObject *parent = nullptr);
    QList<QAbstractSeries*> series() const;
    void updateFromCpu(const SystemCPU& cpuData);

signals:
    void seriesChanged();
private:
    QList<QAbstractSeries*> m_series;
    qint64 m_time = 0;
};

#endif // CPUSTATSVIEWMODEL_H
