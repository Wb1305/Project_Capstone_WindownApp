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
    // Q_PROPERTY(QString totalRAM READ totalRAM NOTIFY infoChanged)
    // Q_PROPERTY(QString totalSWAP READ totalSWAP NOTIFY infoChanged)
    // Q_PROPERTY(QString cacheSize READ cacheSize NOTIFY infoChanged)
public:
    explicit MemStatsViewModel(QObject *parent = nullptr);
    QList<QAbstractSeries*> series() const;
    // QList<QObject*> series() const;
    void updateFromMem(const SystemMEM& memData);

    // QString totalRAM() const;
    // QString totalSWAP() const;
    // QString cacheSize() const;

    void debugPrintSeries() const;

signals:
    void seriesChanged();
    void infoChanged();
private:
    QLineSeries* m_ramSeries = nullptr;
    QLineSeries* m_swapSeries = nullptr;
    QList<QAbstractSeries*> m_series;
    // QList<QObject*> m_series;
    qint64 m_time = 0;
    // QString m_totalRAM;
    // QString m_totalSWAP;
    // QString m_cacheSize;
};

#endif // MEMSTATSVIEWMODEL_H
