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
QDateTime ProcessInfo::timestamp() const { return m_timestamp; }
double ProcessInfo::cpuUsagePercent() const { return m_cpuUsagePercent; }
double ProcessInfo::memUsagePercent() const { return m_memUsagePercent; }
double ProcessInfo::memUsageMB() const { return m_memUsageMB; }

void ProcessInfo::setPID(int pid)
{
    m_pid = pid;
}

void ProcessInfo::setName(const QString &name)
{
    m_name = name;
}

void ProcessInfo::setUser(const QString &user)
{
    m_user = user;
}

void ProcessInfo::setTimestamp(const QDateTime &timestamp)
{
    m_timestamp = timestamp;
}

void ProcessInfo::setCpuPercent(double cpuPercent)
{
    m_cpuUsagePercent = cpuPercent;
}

void ProcessInfo::setMemPercent(double memPercent)
{
    m_memUsagePercent = memPercent;
}

void ProcessInfo::setMemMB(double memMB)
{
    m_memUsageMB = memMB;
}

std::optional<ProcessInfo> ProcessInfo::fromJson(const QJsonObject &obj, const QDateTime &timestamp)
{
    if (!obj.contains("PID") ||
        !obj.contains("User") ||
        !obj.contains("PName") ||
        !obj.contains("PCPUUsagePercent") ||
        !obj.contains("PMEMUsageMB") ||
        !obj.contains("PMEMUsagePercent")) {
        return std::nullopt;
    }

    ProcessInfo procInfo;
    procInfo.setTimestamp(timestamp);
    procInfo.setPID(obj["PID"].toInt());
    procInfo.setUser(obj["User"].toString());
    procInfo.setName(obj["PName"].toString());
    procInfo.setCpuPercent(obj["PCPUUsagePercent"].toDouble());
    procInfo.setMemMB(obj["PMEMUsageMB"].toDouble());
    procInfo.setMemPercent(obj["PMEMUsagePercent"].toDouble());
    return procInfo;
}



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
