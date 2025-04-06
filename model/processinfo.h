#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include<QString>
#include<QDateTime>

class ProcessInfo
{
public:
    ProcessInfo();
    ProcessInfo(int pid, const QString& name, const QString& user,
                const QDateTime& timestamp, double cpuPercent, double memPercent, double memMB);

    int pid() const;
    QString name() const;
    QString user() const;
    QDateTime time() const;
    double cpuUsagePercent() const;
    double memUsagePercent() const;
    double memUsageMB() const;

    QStringList toRow() const;
private:
    int m_pid;
    QString m_name;
    QString m_user;
    QDateTime m_timestamp;
    double m_cpuUsagePercent;
    double m_memUsagePercent;
    double m_memUsageMB;
};

#endif // PROCESSINFO_H
