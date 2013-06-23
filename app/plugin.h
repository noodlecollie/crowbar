#ifndef PLUGIN_H
#define PLUGIN_H

#endif // PLUGIN_H

#include <QString>

namespace Plugins
{
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
        // Returns the plugin's unique ID string.
        virtual QString GetUniqueId() = 0;

        // Returns the plugin's author.
        virtual QString GetAuthor() = 0;

        // Returns the plugin's type.
        virtual PluginType GetType() = 0;

        // Returns the plugin's version.
        virtual PluginVersion GetVersion() = 0;
    };
}
