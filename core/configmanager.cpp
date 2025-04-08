#include "configmanager.h"
#include <QCoreApplication>
#include <QFile>

ConfigManager::ConfigManager(QObject *parent)
    : QObject{parent}
{
    // load("config/ivi_settings.ini");
    QString path = QCoreApplication::applicationDirPath() + "/MyWindownApp/config/ivi_settings.ini";
    if (QFile::exists(path))
        load(path);
    else
        qWarning() << "Config file not found:" << path;
}
// D:\NMPhuong\PROJ_Qt\MyWindownApp\build\Desktop_Qt_6_8_0_MinGW_64_bit-Debug\MyWindownApp\config

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
}
