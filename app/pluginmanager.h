#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QList>

namespace Plugins
{
   class PluginManager : public QObject
    {
        Q_OBJECT
    public:

       // Constructor
       // Input:    QObject* parent Parent widget
       //           QString loadDir Directory from which to load plugins, relative to application path.
        explicit PluginManager(QObject *parent = 0, QString loadDir = "");

       // Destructor
        ~PluginManager();

       // Loads plugins and returns a list of IDs for those successfully loaded.
       // Return:   Qlist   List of successfully loaded plugins.
        QList<QString> LoadPlugins();

        // Sets the directory from which to load plugins.
        // Input:   QString Directory path, relative to application path.
        void SetLoadDir(QString);

    signals:

    public slots:

    protected:

        // Destroys all plugins in the hash table.
        void DeletePluginList();

        // Generates full path from given plugin load directory.
        // Return:  QDir    Full path of application and load directory.
        QDir FullPluginPath();

        QString m_szLoadDir;                    // Path to load plugins from.
        QHash<QString, QObject*>* m_PluginList; // List of all loaded plugins.
    };
}

#endif // PLUGINMANAGER_H
