#include "configviewmodel.h"

ConfigViewModel::ConfigViewModel(ConfigManager *configManager, QObject *parent)
    : QObject{parent},
    m_configManager(configManager)
{
    Q_ASSERT(m_configManager != nullptr); // đảm bảo không null
}

ConfigManager *ConfigViewModel::config() const
{
    return m_configManager;
}

void ConfigViewModel::saveConfig()
{
    m_configManager->save();
}
