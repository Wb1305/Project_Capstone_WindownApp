#include "processlistviewmodel.h"

ProcessListViewModel::ProcessListViewModel(QObject *parent)
    : QAbstractTableModel{parent}
{

    m_roleOrder = { NameRole, PidRole, UserRole, CpuRole, MemRole, MemMBRole };
}

int ProcessListViewModel::rowCount(const QModelIndex &parent) const
{
    return m_processes.size();
}

int ProcessListViewModel::columnCount(const QModelIndex &parent) const
{
    return roleNames().size();
}

QVariant ProcessListViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_processes.size())
        return QVariant();

    const auto &proc = m_processes.at(index.row());
    switch (role) {
    case NameRole: return proc.name();
    case PidRole: return proc.pid();
    case UserRole: return proc.user();
    case CpuRole: return proc.cpuUsagePercent();
    case MemRole: return proc.memUsagePercent();
    case MemMBRole: return proc.memUsageMB();
    // case TimestampRole: return proc.timestamp();
    }
    return QVariant();
}

QVariant ProcessListViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        auto keys = roleNames().keys();
        if (section < keys.size())
            return QString(roleNames().value(keys.at(section)));
    }
    return QVariant();
}

QHash<int, QByteArray> ProcessListViewModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {PidRole, "pid"},
        {UserRole, "user"},
        {CpuRole, "cpu"},
        {MemRole, "mem"},
        {MemMBRole, "memMB"}
        // {TimestampRole, "timestamp"}
    };
}

void ProcessListViewModel::bindToMonitor(SystemMonitor *monitor)
{
    if(!monitor) return;
    QObject::connect(monitor, &SystemMonitor::systemUpdated,
                     this, [this](const SystemStats&, const QVector<ProcessInfo>& listProcess){
                        updateList(listProcess);
                     });
}

QVariant ProcessListViewModel::getData(int row, const QString &roleName) const
{
    int role = roleNames().key(roleName.toUtf8(), -1);
    if (role == -1 || row >= m_processes.size()) return QVariant();
    return data(index(row, 0), role);
}

QString ProcessListViewModel::getHeader(int section) const
{
    // return headerData(section, Qt::Horizontal, Qt::DisplayRole).toString();
    if (section < 0 || section >= m_roleOrder.size())
        return QString();

    int role = m_roleOrder.at(section);
    return QString::fromUtf8(roleNames().value(role));
}

QString ProcessListViewModel::roleNameAt(int index) const
{
    if (index < 0 || index >= m_roleOrder.size())
        return QString();
    return QString::fromUtf8(roleNames().value(m_roleOrder.at(index)));
    // auto keys = roleNames().keys();
    // if (columnIndex < keys.size()) {
    //     return QString::fromUtf8(roleNames().value(keys[columnIndex]));
    // }
    // return QString();
}

void ProcessListViewModel::updateList(const QVector<ProcessInfo> &newListProcess)
{
    beginResetModel();
    m_processes = newListProcess;
    endResetModel();
}

