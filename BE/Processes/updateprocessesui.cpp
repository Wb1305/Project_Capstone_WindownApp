#include "updateprocessesui.h"

UpdateProcessesUI::UpdateProcessesUI(QObject *parent) : QAbstractTableModel(parent)
{}

int UpdateProcessesUI::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return processList.size();
}

int UpdateProcessesUI::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant UpdateProcessesUI::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= processList.size())
        return QVariant();

    const Process &proc = processList.at(index.row());

    switch (role) {
    case Qt::UserRole + 1:
        return proc.name;
    case Qt::UserRole + 2:
        return proc.pid;
    case Qt::UserRole + 3:
        return proc.cpu;
    case Qt::UserRole + 4:
        return proc.memory;
    case Qt::UserRole + 5:
        return proc.disk;
    case Qt::UserRole + 6:
        return proc.network;
    default:
        return QVariant();
    }
    return QVariant();
}

QVariant UpdateProcessesUI::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        static const QString headers[] = { "Name", "PID" ,"CPU", "Memory", "Disk", "Network" };
        return headers[section];
    }
    return QVariant();
}

QHash<int, QByteArray> UpdateProcessesUI::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display"; // Role mặc định
    roles[Qt::UserRole + 1] = "name";
    roles[Qt::UserRole + 2] = "pid";
    roles[Qt::UserRole + 3] = "cpu";
    roles[Qt::UserRole + 4] = "memory";
    roles[Qt::UserRole + 5] = "disk";
    roles[Qt::UserRole + 6] = "network";
    return roles;
}
void UpdateProcessesUI::addProcess(const Process &process) {
    beginInsertRows(QModelIndex(), processList.size(), processList.size());
    processList.append(process);
    endInsertRows();
}

void UpdateProcessesUI::clear() {
    beginResetModel();
    processList.clear();
    endResetModel();
}
