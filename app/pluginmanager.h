/*! \file pluginmanager.h
 * \brief Defines the manager for Crowbar plugins.
 */

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QDir>
#include <QList>

/**
 * @brief Manages loading of plugins.
 */
class PluginManager : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Constructor.
     * @param parent Parent object (usually NULL).
     * @param loadDir Directory from which to load plugins, relative to the application directory.
     */
    explicit PluginManager(QObject *parent = 0, QString loadDir = "");

    /**
     * @brief Destructor.
     */
    ~PluginManager();

    /**
     * @brief Loads plugins and returns a list of IDs for those successfully loaded.
     * @return List of successfully loaded plugins.
     */
    QList<QString> LoadPlugins();

signals:

public slots:
    /**
     * @brief Sets the directory from which to load plugins.
     * @param dir Directory path relative to the application directory.
     */
    void SetLoadDir(QString dir) { m_szLoadDir = dir; }

protected:

    /**
     * @brief Destroys all plugins in the hash table.
     */
    void DeletePluginList();

    /**
     * @brief Generates full path from given plugin load directory.
     * @note If the path does not exist, the returned path will be the application directory.
     * @return Full path of application and load directory.
     */
    QDir FullPluginPath();

    QString m_szLoadDir;                    /**< Path to load plugins from. */
    QHash<QString, QObject*>* m_PluginList; /**< List of all loaded plugins. */
};

#endif // PLUGINMANAGER_H
