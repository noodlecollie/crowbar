#include "pluginmanager.h"
#include <QPluginLoader>
#include <QApplication>

#include "plugin.h"

// Constructor
// Input:    QObject* parent Parent widget
//           QString loadDir Directory from which to load plugins., relative to the application directory.
PluginManager::PluginManager(QObject *parent, QString loadDir) :
    QObject(parent)
{
    m_szLoadDir = loadDir;
    m_PluginList = new QHash<QString, QObject*>();
}

// Destructor
PluginManager::~PluginManager()
{
    if ( m_PluginList ) delete m_PluginList;
}

// Loads plugins and returns a list of IDs for those successfully loaded.
// Return:   Qlist   List of successfully loaded plugins.
QList<QString> PluginManager::LoadPlugins()
{
    DeletePluginList();                 // Delete current plugin list.
    QDir pluginDir = FullPluginPath();  // Get the directory to load from.
    QList<QString> list;                // Create a list to store loaded plugins.

    // Iterate through plugins:
    foreach (QString fileName, pluginDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginDir.absoluteFilePath(fileName)); // Generate full path.
        QObject *plugin = loader.instance();                        // Instance the plugin.

        if (plugin) // If loading succeeded:
        {
            IPlugin *iPlugin = qobject_cast<IPlugin*>(plugin);  // Check if the plugin implements the core interface.

            // If it does and doesn't already exist in the table:
            if ( iPlugin && m_PluginList->find(iPlugin->GetUniqueId()) == m_PluginList->end() )
            {
                m_PluginList->insert(iPlugin->GetUniqueId(), plugin);   // Add to hash table.
                list.append(iPlugin->GetUniqueId());                         // Add ID to our return list.
            }
            else    // The interface is not implemented so we can't load the plugin.
            {
                loader.unload();    // Unload the plugin.
            }
        }
    }

    // Return the list.
    return list;
}

// Destroys all plugins in the hash table.
void PluginManager::DeletePluginList()
{
    QHashIterator<QString, QObject*> it(*m_PluginList);

    while (it.hasNext())
    {
        it.next();
        delete it.value();
    }
}

// Generates full path from given plugin load directory.
// Return:  QDir    Full path of application and load directory.
QDir PluginManager::FullPluginPath()
{
    QDir dir(qApp->applicationDirPath());       // Get the application directory.

    dir.cd(m_szLoadDir);    // If CD doesn't work we'll just remain in the application directory. Is there a batter way to do this?
    return dir;
}
