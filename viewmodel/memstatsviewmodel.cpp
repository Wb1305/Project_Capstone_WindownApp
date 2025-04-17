#include "memstatsviewmodel.h"
#include <QLineSeries>

MemStatsViewModel::MemStatsViewModel(QObject *parent)
    : QObject{parent}
{}


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
        m_ramSeries->append(m_time, qRound(memData.ramPercent()));
        while (m_ramSeries->count() > 0 && m_ramSeries->at(0).x() < m_time - 60)
            m_ramSeries->remove(0);
        // m_ramSeries->setName(QString("Memmory  %1%").arg(memData.ramUtilization(), 0, 'f', 1));
        m_ramSeries->setName(customLegend("Memmory ", memData.ramPercent(), memData.ramUtilization(), memData.maxRamSystem()));
    }

    if (m_swapSeries) {
        m_swapSeries->append(m_time, qRound(memData.swapPercent()));
        while (m_swapSeries->count() > 0 && m_swapSeries->at(0).x() < m_time - 60)
            m_swapSeries->remove(0);
        // m_swapSeries->setName(QString("SWAP  %1%").arg(memData.swapUtilization(), 0, 'f', 1));
        m_swapSeries->setName(customLegend("Swap ", memData.swapPercent(), memData.swapUtilization(), memData.maxSwapSystem()));
    }

    m_ramPercent = memData.ramPercent();
    m_swapPercent = memData.swapPercent();
    m_totalRam = memData.maxRamSystem();
    m_totalSwap = memData.maxSwapSystem();

    ++m_time;
    emit infoChanged();  // cập nhật text hiển thị
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

double MemStatsViewModel::ramPercent() const
{
    return m_ramPercent;
}

double MemStatsViewModel::swapPercent() const
{
    return m_swapPercent;
}

double MemStatsViewModel::totalRam() const
{
    return m_totalRam;
}

double MemStatsViewModel::totalSwap() const
{
    return m_totalSwap;
}

QString MemStatsViewModel::formatSize(double mb)
{
    if (mb < 1.0)
        return "0 bytes";
    if (mb < 1024.0)
        return QString("%1 MB").arg(mb, 0, 'f', 0);
    return QString("%1 GB").arg(mb / 1024.0, 0, 'f', 1);
}

QString MemStatsViewModel::customLegend(QString name, double usagePercent, double usageMB, double totalSys)
{
    // double usage = usageMB / 100;
    QString used = formatSize(usageMB); //MB
    QString total = formatSize(totalSys); //GB
    QString percentStr = QString::number(usagePercent, 'f', 1); //%
    QString nameItem = name;

    QString templateStr = "%1 %2 (%3%) of %4";
    return templateStr.arg(nameItem, used, percentStr, total);
}

