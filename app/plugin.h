/*! \file plugin.h
 * \brief Defines interfaces for Crowbar extension plugins.
 *
 * At its core, every Crowbar plugin must implement the IPlugin interface to be loaded by the application.
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtPlugin>

class QString;

/*!
 * \typedef PluginVersion
 * \brief Typedef for int[4] - holds the plugin's major, minor, revision and build numbers.
 */
typedef int PluginVersion[4];

/**
 * @brief Required core interface for a Crowbar plugin.
 */
class IPlugin
{
public:
    virtual ~IPlugin() {}

    /**
     * @brief Returns the plugin's unique ID string.
     * @note If a loaded plugin with an ID string already exists, subsequent plugins with this ID will fail to load.
     * @return ID string.
     */
    virtual QString GetUniqueId() = 0;

    /**
     * @brief Returns the plugin's author.
     * @return Plugin's author.
     */
    virtual QString GetAuthor() = 0;

    /**
     * @brief Modifies the passed PluginVersion array to return this plugin's version.
     * @param version Array to hold the plugin's version.
     */
    virtual void GetVersion(PluginVersion* version) = 0;
};

//! \def IPlugin_iid
//! \brief Unique ID for IPlugin interface.
#define IPlugin_iid "Crowbar.Interfaces.IPlugin"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

#endif // PLUGIN_H
