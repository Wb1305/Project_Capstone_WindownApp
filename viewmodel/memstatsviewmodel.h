#ifndef MEMSTATSVIEWMODEL_H
#define MEMSTATSVIEWMODEL_H

#include "model/systemmem.h"
#include <QObject>
#include <QAbstractSeries>
#include <QLineSeries>

class MemStatsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QAbstractSeries*> series READ series NOTIFY seriesChanged)
    Q_PROPERTY(double ramPercent READ ramPercent NOTIFY infoChanged)
    Q_PROPERTY(double swapPercent READ swapPercent NOTIFY infoChanged)
    Q_PROPERTY(double totalRam READ totalRam NOTIFY infoChanged)
    Q_PROPERTY(double totalSwap READ totalSwap NOTIFY infoChanged)

public:
    explicit MemStatsViewModel(QObject *parent = nullptr);
    QList<QAbstractSeries*> series() const;
    void updateFromMem(const SystemMEM& memData);

    Q_INVOKABLE QPointF getLatestPoint(int index) const;
    Q_INVOKABLE QPointF getLatestRamPoint() const;
    Q_INVOKABLE QPointF getLatestSwapPoint() const;
    void debugPrintSeries() const;

    double ramPercent() const;
    double swapPercent() const;
    double totalRam() const;
    double totalSwap()const;

    //custom legend
    QString formatSize(double mb);
    QString customLegend(QString name, double usagePercent, double usageMB, double totalSys);

signals:
    void seriesChanged();
    void infoChanged();
private:
    QLineSeries* m_ramSeries = nullptr;
    QLineSeries* m_swapSeries = nullptr;
    QList<QAbstractSeries*> m_series;
    qint64 m_time = 0;
    double m_ramPercent;
    double m_swapPercent;
    double m_totalRam;
    double m_totalSwap;

};

#endif // MEMSTATSVIEWMODEL_H
