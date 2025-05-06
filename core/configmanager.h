#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QString>

class ConfigManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(quint16 serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(int overloadingValue READ overloadingValue WRITE setOverloadingValue NOTIFY overloadingValueChanged)
    Q_PROPERTY(int warningValue READ warningValue WRITE setWarningValue NOTIFY warningValueChanged)
    Q_PROPERTY(double weightRam READ weightRam WRITE setWeightRam NOTIFY weightRamChanged)
    Q_PROPERTY(double weightSwap READ weightSwap WRITE setWeightSwap NOTIFY weightSwapChanged)
    Q_PROPERTY(double weightCpu READ weightCpu WRITE setWeightCpu NOTIFY weightCpuChanged)
    Q_PROPERTY(double weightTemp READ weightTemp WRITE setWeightTemp NOTIFY weightTempChanged)
    Q_PROPERTY(double weightFreq READ weightFreq WRITE setWeightFreq NOTIFY weightFreqChanged)
    Q_PROPERTY(int tempMin READ tempMin WRITE setTempMin NOTIFY tempMinChanged)
    Q_PROPERTY(int tempMax READ tempMax WRITE setTempMax NOTIFY tempMaxChanged)
    Q_PROPERTY(int criticalCpuThreshold READ criticalCpuThreshold WRITE setCriticalCpuThreshold NOTIFY criticalCpuThresholdChanged)
    Q_PROPERTY(int criticalMemThreshold READ criticalMemThreshold WRITE setCriticalMemThreshold NOTIFY criticalMemThresholdChanged)
    Q_PROPERTY(int criticalTempThreshold READ criticalTempThreshold WRITE setCriticalTempThreshold NOTIFY criticalTempThresholdChanged)
    Q_PROPERTY(int criticalDurationSecondsThreshold READ criticalDurationSecondsThreshold WRITE setCriticalDurationSecondsThreshold NOTIFY criticalDurationSecondsThresholdChanged)
    Q_PROPERTY(int overloadCountThreshold READ overloadCountThreshold WRITE setOverloadCountThreshold NOTIFY overloadCountThresholdChanged)
    Q_PROPERTY(int overloadConsecutiveThreshold READ overloadConsecutiveThreshold WRITE setOverloadConsecutiveThreshold NOTIFY overloadConsecutiveThresholdChanged)
    Q_PROPERTY(int overloadDebounceSeconds READ overloadDebounceSeconds WRITE setOverloadDebounceSeconds NOTIFY overloadDebounceSecondsChanged)
    Q_PROPERTY(int warningCountThreshold READ warningCountThreshold WRITE setWarningCountThreshold NOTIFY warningCountThresholdChanged)
    Q_PROPERTY(int warningConsecutiveThreshold READ warningConsecutiveThreshold WRITE setWarningConsecutiveThreshold NOTIFY warningConsecutiveThresholdChanged)
    Q_PROPERTY(int warningDebounceSeconds READ warningDebounceSeconds WRITE setWarningDebounceSeconds NOTIFY warningDebounceSecondsChanged)
    Q_PROPERTY(int potentialOverloadCount READ potentialOverloadCount WRITE setPotentialOverloadCount NOTIFY potentialOverloadCountChanged)

public:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ ConfigManager();

    QVariant getValue(const QString& key) const;
    void setValue(const QString& key, const QVariant& value);

    void load(const QString& path);
    Q_INVOKABLE void save();
    void setupDefaults();
    void printAll() const;

    // --- getter ---
    QString serverIp() const;
    quint16 serverPort() const;
    int overloadingValue() const;
    int warningValue() const;
    double weightRam() const;
    double weightSwap() const;
    double weightCpu() const;
    double weightTemp() const;
    double weightFreq() const;
    int tempMin() const;
    int tempMax() const;
    int criticalCpuThreshold() const;
    int criticalMemThreshold() const;
    int criticalTempThreshold() const;
    int criticalDurationSecondsThreshold() const;
    int overloadCountThreshold() const;
    int overloadConsecutiveThreshold() const;
    int overloadDebounceSeconds() const;
    int warningCountThreshold() const;
    int warningConsecutiveThreshold() const;
    int warningDebounceSeconds() const;
    int potentialOverloadCount() const;

    // --- setter ---
    Q_INVOKABLE void setServerIp(QString value);
    Q_INVOKABLE void setServerPort(quint16 value);
    Q_INVOKABLE void setOverloadingValue(int value);
    Q_INVOKABLE void setWarningValue(int value);
    Q_INVOKABLE void setWeightRam(double value);
    Q_INVOKABLE void setWeightSwap(double value);
    Q_INVOKABLE void setWeightCpu(double value);
    Q_INVOKABLE void setWeightTemp(double value);
    Q_INVOKABLE void setWeightFreq(double value);
    Q_INVOKABLE void setTempMin(int value);
    Q_INVOKABLE void setTempMax(int value);
    Q_INVOKABLE void setCriticalCpuThreshold(int value);
    Q_INVOKABLE void setCriticalMemThreshold(int value);
    Q_INVOKABLE void setCriticalTempThreshold(int value);
    Q_INVOKABLE void setCriticalDurationSecondsThreshold(int value);
    Q_INVOKABLE void setOverloadCountThreshold(int value);
    Q_INVOKABLE void setOverloadConsecutiveThreshold(int value);
    Q_INVOKABLE void setOverloadDebounceSeconds(int value);
    Q_INVOKABLE void setWarningCountThreshold(int value);
    Q_INVOKABLE void setWarningConsecutiveThreshold(int value);
    Q_INVOKABLE void setWarningDebounceSeconds(int value);
    Q_INVOKABLE void setPotentialOverloadCount(int value);

signals:
    void serverIpChanged();
    void serverPortChanged();
    void overloadingValueChanged();
    void warningValueChanged();
    void weightRamChanged();
    void weightSwapChanged();
    void weightCpuChanged();
    void weightTempChanged();
    void weightFreqChanged();
    void tempMinChanged();
    void tempMaxChanged();
    void criticalCpuThresholdChanged();
    void criticalMemThresholdChanged();
    void criticalTempThresholdChanged();
    void criticalDurationSecondsThresholdChanged();
    void overloadCountThresholdChanged();
    void overloadConsecutiveThresholdChanged();
    void overloadDebounceSecondsChanged();
    void warningCountThresholdChanged();
    void warningConsecutiveThresholdChanged();
    void warningDebounceSecondsChanged();
    void potentialOverloadCountChanged();

    // --- test ---
    void configChanged();

private:
    QSettings* m_settings = nullptr;
    QString m_iniPath;

    QString m_serverIp;
    quint16 m_serverPort;
    int m_overloadingValue;
    int m_warningValue;
    double m_weightRam;
    double m_weightSwap;
    double m_weightCpu;
    double m_weightTemp;
    double m_weightFreq;
    int m_tempMin;
    int m_tempMax;
    int m_criticalCpuThreshold;
    int m_criticalMemThreshold;
    int m_criticalTempThreshold;
    int m_criticalDurationSecondsThreshold;
    int m_overloadCountThreshold;
    int m_overloadConsecutiveThreshold;
    int m_overloadDebounceSeconds;
    int m_warningCountThreshold;
    int m_warningConsecutiveThreshold;
    int m_warningDebounceSeconds;
    int m_potentialOverloadCount;
};

#endif // CONFIGMANAGER_H
