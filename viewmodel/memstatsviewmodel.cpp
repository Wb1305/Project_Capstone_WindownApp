#include "memstatsviewmodel.h"
#include <QLineSeries>

MemStatsViewModel::MemStatsViewModel(QObject *parent)
    : QObject{parent}
{}

// QList<QObject *> MemStatsViewModel::series() const
// {

// }

QList<QAbstractSeries *> MemStatsViewModel::series() const
{
    return m_series;
}

void MemStatsViewModel::updateFromMem(const SystemMEM &memData)
{
    // khởi tạo ram series
    if(!m_ramSeries){
        m_ramSeries = new QLineSeries(this);
        m_ramSeries->setName("RAM");
        m_series.append(m_ramSeries);
        // emit seriesChanged();
    }
    // Khởi tạo swap series
    if(!m_swapSeries){
        m_swapSeries = new QLineSeries(this);
        m_swapSeries->setName("SWAP");
        m_series.append(m_swapSeries);
        // emit seriesChanged();
    }
    // Append dữ liệu nếu series đã có
    if (m_ramSeries) {
        m_ramSeries->append(m_time, memData.ramUtilization());
        while (m_ramSeries->count() > 0 && m_ramSeries->at(0).x() < m_time - 60)
            m_ramSeries->remove(0);
        m_ramSeries->setName(QString("RAM  %1%").arg(memData.ramUtilization(), 0, 'f', 1));
    }

    if (m_swapSeries) {
        m_swapSeries->append(m_time, memData.swapUtilization());
        while (m_swapSeries->count() > 0 && m_swapSeries->at(0).x() < m_time - 60)
            m_swapSeries->remove(0);
        m_swapSeries->setName(QString("SWAP  %1%").arg(memData.swapUtilization(), 0, 'f', 1));
    }

    // Cập nhật các thông tin text khác
    // m_totalRAM = QString("%1 GB").arg(memData.maxRamSystem / 1024.0, 0, 'f', 1);
    // m_totalSWAP = QString("%1 GB").arg(memData.maxSwapSystem / 1024.0, 0, 'f', 1);
    // m_cacheSize = QString("%1 MB").arg(memData.cacheMB);

    ++m_time;
    // emit timeChanged();  // nếu bạn bind trục X
    // emit infoChanged();  // cập nhật text hiển thị
    emit seriesChanged();

    // debugPrintSeries();
}

QPointF MemStatsViewModel::getLatestPoint(int index) const
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

QPointF MemStatsViewModel::getLatestRamPoint() const
{
    return getLatestPoint(0); // mem index 0
}

QPointF MemStatsViewModel::getLatestSwapPoint() const
{
    return getLatestPoint(1); // swap index 1
}

void MemStatsViewModel::debugPrintSeries() const
{
    qDebug()<<"---Print MEM Series---";
    if (m_ramSeries) {
        qDebug() << "[RAM Series] Points:" << m_ramSeries->count();
        for (const QPointF& point : m_ramSeries->points()) {
            qDebug() << "  (x =" << point.x() << ", y =" << point.y() << ")";
        }
    }

    if (m_swapSeries) {
        qDebug() << "[SWAP Series] Points:" << m_swapSeries->count();
        for (const QPointF& point : m_swapSeries->points()) {
            qDebug() << "  (x =" << point.x() << ", y =" << point.y() << ")";
        }
    }
}

// QString MemStatsViewModel::totalRAM() const { return m_totalRAM; }
// QString MemStatsViewModel::totalSWAP() const { return m_totalSWAP; }
// QString MemStatsViewModel::cacheSize() const { return m_cacheSize; }
