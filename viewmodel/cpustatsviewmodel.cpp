#include "cpustatsviewmodel.h"
#include <QLineSeries>
#include <QDebug>

CpuStatsViewModel::CpuStatsViewModel(QObject *parent)
    : QObject{parent}
{}

QList<QAbstractSeries *> CpuStatsViewModel::series() const
{
    return m_series;
}

void CpuStatsViewModel::updateFromCpu(const SystemCPU &cpuData)
{
    // Khởi tạo series lần đầu
    if (m_series.isEmpty()) {
        for (int i = 0; i < cpuData.cores().size(); ++i) {
            auto* series = new QLineSeries(this);
            series->setName(QString("CPU%1").arg(i+1));
            m_series.append(series);
        }
        // emit seriesChanged(); // Để QML gán lại series
    }
    for (int i = 0; i < cpuData.cores().size() && i < m_series.size(); ++i) {
        auto* series = qobject_cast<QLineSeries*>(m_series[i]);
        if(!series) continue;
        double util = cpuData.cores()[i].utilization();
        series->append(m_time, util);

        while(series->count() > 0 && series->at(0).x() < m_time - 60){
            series->remove(0);
        }
        series->setName(QString("CPU%1  %2%").arg(i+1).arg(util, 0, 'f', 1));
    }

    ++m_time;

    emit timeChanged();
    emit seriesChanged();

    // debugPrintSeries();
}

int CpuStatsViewModel::time() const
{
    return m_time;
}

void CpuStatsViewModel::debugPrintSeries() const
{
    qDebug()<<"---Print CPU Series---";
    for (int i = 0; i < m_series.size(); ++i) {
        auto* series = qobject_cast<QLineSeries*>(m_series[i]);
        if (!series) continue;
        qDebug() << "Series" << series->name() << ":";
        for (const QPointF& point : series->points()) {
            qDebug() << "  (" << point.x() << "," << point.y() << ")";
        }
    }
}

QList<QPointF> CpuStatsViewModel::getPointsForCore(int index) const
{
    if (index >= 0 && index < m_series.size()) {
        auto* series = qobject_cast<QLineSeries*>(m_series[index]);
        if (series) return series->points();
    }
    return {};
}

QPointF CpuStatsViewModel::getLatestPoint(int index) const
{
    if (index >= 0 && index < m_series.size()) {
        auto* series = qobject_cast<QLineSeries*>(m_series[index]);
        if (series) {
            const auto& points = series->points();
            if (!points.isEmpty())
                return points.constLast();
        }
    }
    return {};
}
