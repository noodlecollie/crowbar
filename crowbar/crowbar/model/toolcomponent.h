#ifndef TOOLCOMPONENT_H
#define TOOLCOMPONENT_H

#include "mapcomponent.h"
#include <QString>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT ToolComponent : public MapComponent
{
    Q_OBJECT
    MAPCOMPONENT_SUBCLASS(ToolComponent, MapComponent)

    Q_PROPERTY(QString ownerTool READ ownerTool WRITE setOwnerTool NOTIFY ownerToolChanged)
    Q_PROPERTY(int visibleRenderModes READ visibleRenderModes WRITE setVisibleRenderModes NOTIFY visibleRenderModesChanged)
public:
    enum VisibleRenderModes
    {
        Triangles   = 0x1,
        Edges       = 0x2
    };

    QString ownerTool() const;
    void setOwnerTool(const QString &toolName);

    int visibleRenderModes() const;
    void setVisibleRenderModes(int modes);

signals:
    void ownerToolChanged(const QString&);
    void visibleRenderModesChanged(int);

public slots:

private:
    QString m_szOwnerTool;
    int     m_iVisibleRenderModes;
};

MODEL_END_NAMESPACE

#endif // TOOLCOMPONENT_H
