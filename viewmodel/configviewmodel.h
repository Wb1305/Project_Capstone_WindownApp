#ifndef CONFIGVIEWMODEL_H
#define CONFIGVIEWMODEL_H

#include "core/configmanager.h"
#include <QObject>

class ConfigViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ConfigManager* config READ config CONSTANT)

public:
    explicit ConfigViewModel(ConfigManager *configManager, QObject *parent = nullptr);

    ConfigManager* config() const;
    Q_INVOKABLE void saveConfig();
signals:

private:
    ConfigManager* m_configManager;
};

#endif // CONFIGVIEWMODEL_H
