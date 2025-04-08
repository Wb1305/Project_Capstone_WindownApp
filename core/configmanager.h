#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QSettings>
#include <QVariant>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);

    QVariant getValue(const QString& key) const;
    void setValue(const QString& key, const QVariant& value);
    void load(const QString& path);

signals:

private:
    QSettings* m_settings = nullptr;
};

#endif // CONFIGMANAGER_H
