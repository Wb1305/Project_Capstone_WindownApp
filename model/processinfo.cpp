#include "processinfo.h"

ProcessInfo::ProcessInfo() {}
ProcessInfo::ProcessInfo(int pid, const QString& name, const QString& user,
                         const QDateTime& timestamp,
                         double cpuPercent, double memPercent, double memMB)
    : m_pid(pid),
    m_name(name),
    m_user(user),
    m_timestamp(timestamp),
    m_cpuUsagePercent(cpuPercent),
    m_memUsagePercent(memPercent),
    m_memUsageMB(memMB)
{}

int ProcessInfo::pid() const
{
    return m_pid;
}
QString ProcessInfo::name() const { return m_name; }
QString ProcessInfo::user() const { return m_user; }
QDateTime ProcessInfo::time() const { return m_timestamp; }
double ProcessInfo::cpuUsagePercent() const { return m_cpuUsagePercent; }
double ProcessInfo::memUsagePercent() const { return m_memUsagePercent; }
double ProcessInfo::memUsageMB() const { return m_memUsageMB; }

QStringList ProcessInfo::toRow() const
{
    return {
        m_timestamp.toString(Qt::ISODate),
        QString::number(m_pid),
        m_name,
        m_user,
        QString::number(m_cpuUsagePercent, 'f', 2),
        QString::number(m_memUsagePercent, 'f', 2),
        QString::number(m_memUsageMB, 'f', 2)
    };
}
