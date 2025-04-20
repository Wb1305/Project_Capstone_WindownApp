#ifndef PROCESSLISTVIEWMODEL_H
#define PROCESSLISTVIEWMODEL_H

#include "core/systemmonitor.h"
#include "model/processinfo.h"
#include <QObject>
#include <QAbstractTableModel>
#include <QVector>
#include <QList>

class ProcessListViewModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int columnCount READ columnCount CONSTANT)
    Q_PROPERTY(double totalCpuUsagePercent READ totalCpuUsagePercent NOTIFY totalCpuUsageChanged)
    Q_PROPERTY(double totalRamUsagePercent READ totalRamUsagePercent NOTIFY totalRamUsageChanged)
    Q_PROPERTY(double maxRam READ maxRam NOTIFY maxRamChanged)
    Q_PROPERTY(QString currentSortRole READ currentSortRole NOTIFY sortChanged)
    Q_PROPERTY(bool sortAscendingStatus READ sortAscendingStatus NOTIFY sortChanged)

public:
    explicit ProcessListViewModel(QObject *parent = nullptr);

    enum Roles{
        NameRole = Qt::UserRole + 1,
        PidRole,
        UserRole,
        CpuRole,
        MemRole,
        MemMBRole
        // TimestampRole
    };

    enum class SortRole {
        None,
        Cpu,
        Mem,
        MemMB
    };

    Q_ENUM(Roles)  // Cho phép truy cập enum từ QML nếu cần
    Q_ENUM(SortRole)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    //=== connect to monitor: lay data newprocess
    void bindToMonitor(SystemMonitor* monitor);

    //can get value from QML
    Q_INVOKABLE QVariant getData(int row, const QString &roleName) const;
    Q_INVOKABLE QString getHeader(int section) const;
    Q_INVOKABLE QString roleNameAt(int index) const;

    // sort theo rolename
    Q_INVOKABLE void sortBy(const QString& roleName);
    SortRole toSortRole(const QString& roleName) const;
    double extractSortValue(const ProcessInfo& proc, SortRole role) const;
    QString currentSortRole() const;
    bool sortAscendingStatus() const;
    void resetSort();
    void applySort();

    // get value total cpu, ram
    double totalCpuUsagePercent() const;
    double totalRamUsagePercent() const;
    double maxRam() const;
    double totalRamUsaged() const;

    // print to terminal
    void printtest();

signals:
    void totalCpuUsageChanged();
    void totalRamUsageChanged();
    void maxRamChanged();
    void sortChanged();

private slots:
    void updateList(const SystemStats &stats, const QVector<ProcessInfo>& newListProcess);
    // void updateList(const QVector<ProcessInfo>& newListProcess);

private:
    QVector<ProcessInfo> m_processes;
    QVector<ProcessInfo> m_originalProcesses;
    SystemStats m_currentSystemStats;
    QList<int> m_roleOrder;
    SortRole m_sortRole = SortRole::None;
    bool m_sortAscending = true;
    QString m_sortRoleName;

    double m_maxRam;
};

#endif // PROCESSLISTVIEWMODEL_H
