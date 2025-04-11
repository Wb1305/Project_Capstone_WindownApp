#include "processlistviewmodel.h"
#include <QDebug>

ProcessListViewModel::ProcessListViewModel(QObject *parent)
    : QAbstractTableModel{parent}
{

    m_roleOrder = { NameRole, PidRole, UserRole, CpuRole, MemMBRole };
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
    // case MemRole: return proc.memUsagePercent();
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
        // {MemRole, "mem"},
        {MemMBRole, "memMB"}
        // {TimestampRole, "timestamp"}
    };
}

void ProcessListViewModel::bindToMonitor(SystemMonitor *monitor)
{
    if(!monitor) return;
    QObject::connect(monitor, &SystemMonitor::systemUpdated,
                     this, [this](const SystemStats& systemStats, const QVector<ProcessInfo>& listProcess){
                        updateList(systemStats, listProcess);
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
}

void ProcessListViewModel::sortBy(const QString &roleName)
{
    SortRole newRole = toSortRole(roleName);
    if(newRole == SortRole::None) return;

    if(newRole == m_sortRole){
        m_sortAscending = !m_sortAscending;
    } else{
        m_sortRole = newRole;
        m_sortAscending = false;
    }

    std::sort(m_processes.begin(), m_processes.end(), [=](const ProcessInfo& proc1, const ProcessInfo& proc2){
        double lhs = extractSortValue(proc1, m_sortRole);
        double rhs = extractSortValue(proc2, m_sortRole);
        return m_sortAscending ? (lhs < rhs) : (lhs > rhs);
    });

    beginResetModel();
    endResetModel();

    m_sortRoleName = roleName;
    emit sortChanged();

}

ProcessListViewModel::SortRole ProcessListViewModel::toSortRole(const QString &roleName) const
{
    if(roleName == "cpu") return SortRole::Cpu;
    if(roleName == "memMB") return SortRole::MemMB;
    return SortRole::None;
}

double ProcessListViewModel::extractSortValue(const ProcessInfo &proc, SortRole role) const
{
    switch (role) {
    case SortRole::Cpu: return proc.cpuUsagePercent();
    case SortRole::MemMB: return proc.memUsageMB();
    default: return 0.0;
    }
}

QString ProcessListViewModel::currentSortRole() const
{
    return m_sortRoleName;
}

bool ProcessListViewModel::sortAscendingStatus() const
{
    return m_sortAscending;
}



double ProcessListViewModel::totalCpuUsagePercent() const
{
    return m_currentSystemStats.cpuStats().general().utilization();
}

double ProcessListViewModel::totalRamUsagePercent() const
{
    return m_currentSystemStats.memStats().ramUtilization();
}

void ProcessListViewModel::printtest()
{
    qDebug()<<"=== System Usage ===";
    qDebug()<<"Total CPU: " << totalCpuUsagePercent();
    qDebug()<<"Total RAM: " << totalRamUsagePercent();
}

void ProcessListViewModel::updateList(const SystemStats &systempStats, const QVector<ProcessInfo> &newListProcess)
{
    beginResetModel();
    m_processes = newListProcess;

    if(!qFuzzyCompare(totalCpuUsagePercent(), systempStats.cpuStats().general().utilization()))
        emit totalCpuUsageChanged();

    if(!qFuzzyCompare(totalRamUsagePercent(), systempStats.memStats().ramUtilization()))
        emit totalRamUsageChanged();

    m_currentSystemStats = systempStats;

    // Nếu đang có sort (do người dùng đã sort trước đó) → sort lại
    if (m_sortRole != SortRole::None) {
        std::sort(m_processes.begin(), m_processes.end(), [=](const ProcessInfo& a, const ProcessInfo& b) {
            double lhs = extractSortValue(a, m_sortRole);
            double rhs = extractSortValue(b, m_sortRole);
            return m_sortAscending ? (lhs < rhs) : (lhs > rhs);
        });
    }

    endResetModel();
}

