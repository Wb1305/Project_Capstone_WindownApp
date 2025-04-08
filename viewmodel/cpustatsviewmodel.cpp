#include "cpustatsviewmodel.h"
#include <QLineSeries>

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
        emit seriesChanged(); // Để QML gán lại series
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
    // emit seriesChanged();
}
