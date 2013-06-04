#include "pluginmanager.h"

namespace Plugins
{
    PluginManager::PluginManager(QObject *parent) :
        QObject(parent)
    {
        m_szLoadDir = "";
        m_PluginList = new QHash();
    }

    PluginManager::~PluginManager()
    {
        if ( m_PluginList ) delete m_PluginList;
    }
}
