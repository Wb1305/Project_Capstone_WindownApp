#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include<QString>
#include<QDateTime>
#include<QJsonObject>

class ProcessInfo
{
public:
    ProcessInfo();
    ProcessInfo(int pid, const QString& name, const QString& user,
                const QDateTime& timestamp, double cpuPercent, double memPercent, double memMB);

    int pid() const;
    QString name() const;
    QString user() const;
    QDateTime timestamp() const;
    double cpuUsagePercent() const;
    double memUsagePercent() const;
    double memUsageMB() const;

    void setPID(int pid);
    void setName(const QString& name);
    void setUser(const QString& user);
    void setTimestamp(const QDateTime& timestamp);
    void setCpuPercent(double cpuPercent);
    void setMemPercent(double memPercent);
    void setMemMB(double memMB);

    // bool fromJson(const QJsonObject& obj, const QDateTime& timestamp);
    static std::optional<ProcessInfo> fromJson(const QJsonObject& obj, const QDateTime& timestamp);
    QStringList toRow() const;
private:
    int m_pid = -1;
    QString m_name;
    QString m_user;
    QDateTime m_timestamp;
    double m_cpuUsagePercent = 0;
    double m_memUsagePercent = 0;
    double m_memUsageMB = 0;
};

#endif // PROCESSINFO_H
