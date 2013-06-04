#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QString>
#include <QHash>

namespace Plugins
{
    enum Type
    {
        // Invalid
        TypeNone = -1,

        // Doctype plugin - defines objects that can exist in a document.
        // Implements IPluginDoctype.
        TypeDoc,

        // Render protocol plugin - defines conversions from doctype plugin(s) to
        // renderable forms. Implements IProtocolRender.
        TypeRenderProtocol,

        // Tool plugin - defines tool(s) which are operation that can be applied to
        // map objects. Implements IPluginTool.
        TypeTool,

        // Serialise protocol plugin - defines de/serialisations from doctype plugin(s)
        // when importing or exporting. Implements IProtocolSerialise.
        TypeSerialiseProtocol
    };

    class PluginManager : public QObject
    {
        Q_OBJECT
    public:
        explicit PluginManager(QObject *parent = 0);
        ~PluginManager();

    signals:

    public slots:

    protected:
        QString m_szLoadDir;                    // Path to load plugins from.
        QHash<QString, QObject*>* m_PluginList; // List of all loaded plugins.
    };
}

#endif // PLUGINMANAGER_H
