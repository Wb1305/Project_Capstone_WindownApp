#include "dataprocessor.h"
#include <QJsonParseError>

DataProcessor::DataProcessor(QObject *parent)
    : QObject{parent}
{}

const SystemStats &DataProcessor::systemStats() const
{
    return m_systemStats;
}

const QVector<ProcessInfo> &DataProcessor::processList() const
{
    return m_processes;
}

QDateTime DataProcessor::timestamp() const
{
    return m_timestamp;
}

bool DataProcessor::parseJsonData(const QByteArray &jsonData)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &err);
    if(err.error != QJsonParseError::NoError || !doc.isObject()){
        qWarning() << "Invalid JSON:" << err.errorString();
        return false;
    }

    QJsonObject root = doc.object();
    if(!root.contains("timestamp")){
        qWarning() << "[DataProcessor] Missing timestamp field";
        return false;
    }

    QDateTime ts = QDateTime::fromString(root["timestamp"].toString(), "yyyy-MM-dd HH:mm:ss");
    m_timestamp = ts;
    m_systemStats.setTimestamp(ts);

    // parse system stats
    if (!m_systemStats.fromJson(root)) {
        qWarning() << "[DataProcessor] Failed to parse SystemStats";
        return false;
    }
    // parse processes list
    m_processes.clear();
    if (root.contains("ProcessesStats")) {
        QJsonObject procObj = root["ProcessesStats"].toObject();
        for (const QString& key : procObj.keys()) {
            QJsonObject proc = procObj[key].toObject();

            auto procOpt = ProcessInfo::fromJson(proc, ts);
            if(!procOpt.has_value()){
                qWarning() << "[DataProcessor] Skipped process" << key << "due to invalid fields";
            }
            m_processes.append(procOpt.value());
        }
    } else {
        qWarning() << "[DataProcessor] Missing ProcessesStats field";
    }

    // emit dataUpdated();
    emit parseCompleted();

    return true;
}
