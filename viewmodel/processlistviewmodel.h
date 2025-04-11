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

    Q_ENUM(Roles)  // Cho phép truy cập enum từ QML nếu cần

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    //=== connect to monitor: lay data newprocess
    void bindToMonitor(SystemMonitor* monitor);

    // void addProcess(const ProcessInfo &proc);

    // void clear();

    // Q_INVOKABLE int roleForName(const QString &roleName) const {
    //     return roleNames().key(roleName.toUtf8(), -1);
    // }

    Q_INVOKABLE QVariant getData(int row, const QString &roleName) const;
    Q_INVOKABLE QString getHeader(int section) const;
    Q_INVOKABLE QString roleNameAt(int index) const;

signals:

private slots:
    void updateList(const QVector<ProcessInfo>& newListProcess);

private:
    QVector<ProcessInfo> m_processes;
    QList<int> m_roleOrder;
};

#endif // PROCESSLISTVIEWMODEL_H
