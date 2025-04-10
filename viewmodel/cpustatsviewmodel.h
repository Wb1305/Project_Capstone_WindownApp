#ifndef CPUSTATSVIEWMODEL_H
#define CPUSTATSVIEWMODEL_H

#include "model/systemcpu.h"
#include <QObject>
#include <QAbstractSeries>

class CpuStatsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QAbstractSeries*> series READ series NOTIFY seriesChanged);
    Q_PROPERTY(int time READ time NOTIFY timeChanged)
public:
    explicit CpuStatsViewModel(QObject *parent = nullptr);
    QList<QAbstractSeries*> series() const;
    void updateFromCpu(const SystemCPU& cpuData);
    int time() const;

    void debugPrintSeries() const;

    Q_INVOKABLE QList<QPointF> getPointsForCore(int index) const;
    Q_INVOKABLE QPointF getLatestPoint(int index) const;

signals:
    void seriesChanged();
    void timeChanged();
private:
    QList<QAbstractSeries*> m_series;
    int m_time = 0;
};

#endif // CPUSTATSVIEWMODEL_H
