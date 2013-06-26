#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtPlugin>

class QString;

// Herbius: Was going to have a namespace here but it messes with Q_DECLARE_INTERFACE so I removed it.

// Plugin version: Major|Minor|Revision|Build
typedef int PluginVersion[4];

enum PluginType
{
    // Invalid
    TypeNone = -1,

    // Tool plugin - defines tool(s) which are operation that can be applied to
    // map objects. Implements IPluginTool.
    TypeTool,

    // Serialise protocol plugin - defines de/serialisations of map objects
    // when importing or exporting. Implements IProtocolSerialise.
    TypeProtocolSerialise
};

// Required core interface for a plugin.
class IPlugin
{
public:
    virtual ~IPlugin() {}

    // Returns the plugin's unique ID string.
    virtual QString GetUniqueId() = 0;

    // Returns the plugin's author.
    virtual QString GetAuthor() = 0;

    // Returns the plugin's type.
    virtual PluginType GetType() = 0;

    // Modifies the passed PluginVersion array to return this plugin's version.
    virtual void GetVersion(PluginVersion &) = 0;
};

#define IPlugin_iid "Crowbar.Interfaces.IPlugin"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

#endif // PLUGIN_H
