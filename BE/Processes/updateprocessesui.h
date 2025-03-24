#ifndef UPDATEPROCESSESUI_H
#define UPDATEPROCESSESUI_H

#include<QAbstractTableModel>
#include<QVector>

struct Process {
    QString name;
    QString pid;
    QString cpu;
    QString memory;
    QString disk;
    QString network;
};

class UpdateProcessesUI : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit UpdateProcessesUI(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addProcess(const Process &process);
    void clear();

    Q_INVOKABLE int roleForName(const QString &roleName) const {
        return roleNames().key(roleName.toUtf8(), -1);
    }

    Q_INVOKABLE QVariant getData(int row, const QString &roleName) const {
        int role = roleForName(roleName);
        if (role == -1) return QVariant();
        return data(index(row, 0), role);
    }

    // Trong class UpdateProcessesUI
    Q_INVOKABLE QString header(int section) const {
        return headerData(section, Qt::Horizontal, Qt::DisplayRole).toString();
    }


private:
    QVector<Process> processList;
};

#endif // UPDATEPROCESSESUI_H
