#include "configmanager.h"
#include "./setting/configkeys.h"
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QDebug>


ConfigManager::ConfigManager(QObject *parent)
    : QObject{parent}
{

    m_iniPath  = QCoreApplication::applicationDirPath() + "/MyWindownApp/config/my_ivi_settings.ini";
    QDir().mkpath(QFileInfo(m_iniPath).absolutePath());

    // Nếu file chưa tồn tại → tạo settings object trước, rồi gán mặc định và lưu
    if (!QFile::exists(m_iniPath)) {
        qDebug()<<"[ConfigManager] setup and load file config first time!";
        load(m_iniPath);
        setupDefaults();
    } else {
        qDebug()<<"[ConfigManager] Load file config!: "<<m_iniPath;
        load(m_iniPath);
    }
}

ConfigManager::~ConfigManager()
{
    if(m_settings){
        delete m_settings;
    }
}

QVariant ConfigManager::getValue(const QString &key) const
{
    if(!m_settings)
        return QVariant();
    return m_settings->value(key);
}

void ConfigManager::setValue(const QString &key, const QVariant &value)
{
    if(!m_settings)
        return;
    m_settings->setValue(key, value);
}

void ConfigManager::load(const QString &path)
{
    if(m_settings)
        delete m_settings;
    m_settings = new QSettings(path, QSettings::IniFormat);

    m_serverIp = m_settings->value(MyConfigKeys::ServerIp, m_serverIp).toString();
    m_serverPort = m_settings->value(MyConfigKeys::ServerPort, m_serverPort).toInt();
    m_overloadingValue = m_settings->value(MyConfigKeys::OverloadingValue, m_overloadingValue).toInt();
    m_warningValue = m_settings->value(MyConfigKeys::WarningValue, m_warningValue).toInt();
    m_weightRam = m_settings->value(MyConfigKeys::WeightRam, m_weightRam).toDouble();
    m_weightSwap = m_settings->value(MyConfigKeys::WeightSwap, m_weightSwap).toDouble();
    m_weightCpu = m_settings->value(MyConfigKeys::WeightCpu, m_weightCpu).toDouble();
    m_weightTemp = m_settings->value(MyConfigKeys::WeightTemp, m_weightTemp).toDouble();
    m_weightFreq = m_settings->value(MyConfigKeys::WeightFreq, m_weightFreq).toDouble();
    m_tempMin = m_settings->value(MyConfigKeys::TempMin, m_tempMin).toInt();
    m_tempMax = m_settings->value(MyConfigKeys::TempMax, m_tempMax).toInt();
    m_criticalCpuThreshold = m_settings->value(MyConfigKeys::CriticalCpuThreshold, m_criticalCpuThreshold).toInt();
    m_criticalMemThreshold = m_settings->value(MyConfigKeys::CriticalMemThreshold, m_criticalMemThreshold).toInt();
    m_criticalTempThreshold = m_settings->value(MyConfigKeys::CriticalTempThreshold, m_criticalTempThreshold).toInt();
    m_criticalDurationSecondsThreshold = m_settings->value(MyConfigKeys::CriticalDurationSecondsThreshold, m_criticalDurationSecondsThreshold).toInt();
    m_overloadCountThreshold = m_settings->value(MyConfigKeys::OverloadCountThreshold, m_overloadCountThreshold).toInt();
    m_overloadConsecutiveThreshold = m_settings->value(MyConfigKeys::OverloadConsecutiveThreshold, m_overloadConsecutiveThreshold).toInt();
    m_overloadDebounceSeconds = m_settings->value(MyConfigKeys::OverloadDebounceSeconds, m_overloadDebounceSeconds).toInt();
    m_warningCountThreshold = m_settings->value(MyConfigKeys::WarningCountThreshold, m_warningCountThreshold).toInt();
    m_warningConsecutiveThreshold = m_settings->value(MyConfigKeys::WarningConsecutiveThreshold, m_warningConsecutiveThreshold).toInt();
    m_warningDebounceSeconds = m_settings->value(MyConfigKeys::WarningDebounceSeconds, m_warningDebounceSeconds).toInt();
    m_potentialOverloadCount = m_settings->value(MyConfigKeys::PotentialOverloadCount, m_potentialOverloadCount).toInt();

}

void ConfigManager::save()
{
    if(!m_settings) return;

    // using namespace ConfigKeys;
    m_settings->setValue(MyConfigKeys::ServerIp, m_serverIp);
    m_settings->setValue(MyConfigKeys::ServerPort, m_serverPort);
    m_settings->setValue(MyConfigKeys::OverloadingValue, m_overloadingValue);
    m_settings->setValue(MyConfigKeys::WarningValue, m_warningValue);
    m_settings->setValue(MyConfigKeys::WeightRam, m_weightRam);
    m_settings->setValue(MyConfigKeys::WeightSwap, m_weightSwap);
    m_settings->setValue(MyConfigKeys::WeightCpu, m_weightCpu);
    m_settings->setValue(MyConfigKeys::WeightTemp, m_weightTemp);
    m_settings->setValue(MyConfigKeys::WeightFreq, m_weightFreq);
    m_settings->setValue(MyConfigKeys::TempMin, m_tempMin);
    m_settings->setValue(MyConfigKeys::TempMax, m_tempMax);
    m_settings->setValue(MyConfigKeys::CriticalCpuThreshold, m_criticalCpuThreshold);
    m_settings->setValue(MyConfigKeys::CriticalMemThreshold, m_criticalMemThreshold);
    m_settings->setValue(MyConfigKeys::CriticalTempThreshold, m_criticalTempThreshold);
    m_settings->setValue(MyConfigKeys::CriticalDurationSecondsThreshold, m_criticalDurationSecondsThreshold);
    m_settings->setValue(MyConfigKeys::OverloadCountThreshold, m_overloadCountThreshold);
    m_settings->setValue(MyConfigKeys::OverloadConsecutiveThreshold, m_overloadConsecutiveThreshold);
    m_settings->setValue(MyConfigKeys::OverloadDebounceSeconds, m_overloadDebounceSeconds);
    m_settings->setValue(MyConfigKeys::WarningCountThreshold, m_warningCountThreshold);
    m_settings->setValue(MyConfigKeys::WarningConsecutiveThreshold, m_warningConsecutiveThreshold);
    m_settings->setValue(MyConfigKeys::WarningDebounceSeconds, m_warningDebounceSeconds);
    m_settings->setValue(MyConfigKeys::PotentialOverloadCount, m_potentialOverloadCount);

    m_settings->sync();

}

void ConfigManager::setupDefaults()
{
    m_serverIp = "172.20.0.128";
    m_serverPort = 8000;
    m_overloadingValue = 50;
    m_warningValue = 45;

    m_weightRam = 0.35;
    m_weightSwap = 0.25;
    m_weightCpu = 0.25;
    m_weightTemp = 0.10;
    m_weightFreq = 0.05;

    m_tempMin = 40;
    m_tempMax = 90;

    m_criticalCpuThreshold = 85;
    m_criticalMemThreshold = 180;
    m_criticalTempThreshold = 90;
    m_criticalDurationSecondsThreshold = 120;

    m_overloadCountThreshold = 40;
    m_overloadConsecutiveThreshold = 30;
    m_overloadDebounceSeconds = 10;

    m_warningCountThreshold = 20;
    m_warningConsecutiveThreshold = 15;
    m_warningDebounceSeconds = 10;

    m_potentialOverloadCount = 45;

    // Lưu vào file
    save();
}

void ConfigManager::printAll() const
{
    if (!m_settings) {
        qWarning() << "[ConfigManager] Haven't QSettings to read.";
        return;
    }

    qDebug() << "========== [ConfigManager] Dump Settings ==========";

    const QStringList groups = m_settings->childGroups();
    for (const QString& group : groups) {
        qDebug() << "[" << group << "]";
        m_settings->beginGroup(group);

        const QStringList keys = m_settings->childKeys();
        for (const QString& key : keys) {
            QVariant value = m_settings->value(key);
            qDebug() << key << "=" << value.toString();
        }

        m_settings->endGroup();
    }

    qDebug() << "===================================================";
}

// --- getter ---
QString ConfigManager::serverIp() const { return m_serverIp; }
quint16 ConfigManager::serverPort() const { return m_serverPort; }
int ConfigManager::overloadingValue() const { return m_overloadingValue; }
int ConfigManager::warningValue() const { return m_warningValue; }
double ConfigManager::weightRam() const { return m_weightRam; }
double ConfigManager::weightSwap() const { return m_weightSwap; }
double ConfigManager::weightCpu() const { return m_weightCpu; }
double ConfigManager::weightTemp() const { return m_weightTemp; }
double ConfigManager::weightFreq() const { return m_weightFreq; }
int ConfigManager::tempMin() const { return m_tempMin; }
int ConfigManager::tempMax() const { return m_tempMax; }
int ConfigManager::criticalCpuThreshold() const { return m_criticalCpuThreshold; }
int ConfigManager::criticalMemThreshold() const { return m_criticalMemThreshold; }
int ConfigManager::criticalTempThreshold() const { return m_criticalTempThreshold; }
int ConfigManager::criticalDurationSecondsThreshold() const { return m_criticalDurationSecondsThreshold; }
int ConfigManager::overloadCountThreshold() const { return m_overloadCountThreshold; }
int ConfigManager::overloadConsecutiveThreshold() const { return m_overloadConsecutiveThreshold; }
int ConfigManager::overloadDebounceSeconds() const { return m_overloadDebounceSeconds; }
int ConfigManager::warningCountThreshold() const { return m_warningCountThreshold; }
int ConfigManager::warningConsecutiveThreshold() const { return m_warningConsecutiveThreshold; }
int ConfigManager::warningDebounceSeconds() const { return m_warningDebounceSeconds; }
int ConfigManager::potentialOverloadCount() const { return m_potentialOverloadCount; }


// --- setter ---
void ConfigManager::setServerIp(QString value) {
    if (m_serverIp != value) {
        m_serverIp = value;
        emit serverIpChanged();
    }
}

void ConfigManager::setServerPort(quint16 value) {
    if (m_serverPort != value) {
        m_serverPort = value;
        emit serverPortChanged();
    }
}

void ConfigManager::setOverloadingValue(int value) {
    if (m_overloadingValue != value) {
        m_overloadingValue = value;
        emit overloadingValueChanged();
    }
}

void ConfigManager::setWarningValue(int value) {
    if (m_warningValue != value) {
        m_warningValue = value;
        emit warningValueChanged();
    }
}

void ConfigManager::setWeightRam(double value) {
    if (m_weightRam != value) {
        m_weightRam = value;
        emit weightRamChanged();
    }
}

void ConfigManager::setWeightSwap(double value) {
    if (m_weightSwap != value) {
        m_weightSwap = value;
        emit weightSwapChanged();
    }
}

void ConfigManager::setWeightCpu(double value) {
    if (m_weightCpu != value) {
        m_weightCpu = value;
        emit weightCpuChanged();
    }
}

void ConfigManager::setWeightTemp(double value) {
    if (m_weightTemp != value) {
        m_weightTemp = value;
        emit weightTempChanged();
    }
}

void ConfigManager::setWeightFreq(double value) {
    if (m_weightFreq != value) {
        m_weightFreq = value;
        emit weightFreqChanged();
    }
}

void ConfigManager::setTempMin(int value) {
    if (m_tempMin != value) {
        m_tempMin = value;
        emit tempMinChanged();
    }
}

void ConfigManager::setTempMax(int value) {
    if (m_tempMax != value) {
        m_tempMax = value;
        emit tempMaxChanged();
    }
}

void ConfigManager::setCriticalCpuThreshold(int value) {
    if (m_criticalCpuThreshold != value) {
        m_criticalCpuThreshold = value;
        emit criticalCpuThresholdChanged();
    }
}

void ConfigManager::setCriticalMemThreshold(int value) {
    if (m_criticalMemThreshold != value) {
        m_criticalMemThreshold = value;
        emit criticalMemThresholdChanged();
    }
}

void ConfigManager::setCriticalTempThreshold(int value) {
    if (m_criticalTempThreshold != value) {
        m_criticalTempThreshold = value;
        emit criticalTempThresholdChanged();
    }
}

void ConfigManager::setCriticalDurationSecondsThreshold(int value) {
    if (m_criticalDurationSecondsThreshold != value) {
        m_criticalDurationSecondsThreshold = value;
        emit criticalDurationSecondsThresholdChanged();
    }
}

void ConfigManager::setOverloadCountThreshold(int value) {
    if (m_overloadCountThreshold != value) {
        m_overloadCountThreshold = value;
        emit overloadCountThresholdChanged();
    }
}

void ConfigManager::setOverloadConsecutiveThreshold(int value) {
    if (m_overloadConsecutiveThreshold != value) {
        m_overloadConsecutiveThreshold = value;
        emit overloadConsecutiveThresholdChanged();
    }
}

void ConfigManager::setOverloadDebounceSeconds(int value) {
    if (m_overloadDebounceSeconds != value) {
        m_overloadDebounceSeconds = value;
        emit overloadDebounceSecondsChanged();
    }
}

void ConfigManager::setWarningCountThreshold(int value) {
    if (m_warningCountThreshold != value) {
        m_warningCountThreshold = value;
        emit warningCountThresholdChanged();
    }
}

void ConfigManager::setWarningConsecutiveThreshold(int value) {
    if (m_warningConsecutiveThreshold != value) {
        m_warningConsecutiveThreshold = value;
        emit warningConsecutiveThresholdChanged();
    }
}

void ConfigManager::setWarningDebounceSeconds(int value) {
    if (m_warningDebounceSeconds != value) {
        m_warningDebounceSeconds = value;
        emit warningDebounceSecondsChanged();
    }
}

void ConfigManager::setPotentialOverloadCount(int value) {
    if (m_potentialOverloadCount != value) {
        m_potentialOverloadCount = value;
        emit potentialOverloadCountChanged();
    }
}


