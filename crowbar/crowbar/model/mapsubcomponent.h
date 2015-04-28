#ifndef MAPSUBCOMPONENT_H
#define MAPSUBCOMPONENT_H

#include <QObject>
#include "model_global.h"
#include "mapcomponent.h"
#include "changesignal.h"

MODEL_BEGIN_NAMESPACE\

class MapDocument;

class MODELSHARED_EXPORT MapSubComponent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedStatusChanged)
public:
    explicit MapSubComponent(MapComponent *parent = 0);
    virtual ~MapSubComponent();

    inline MapComponent* parentComponent() const
    {
        return qobject_cast<MapComponent*>(parent());
    }

    void setParentComponent(MapComponent* parent);

    MapDocument* document() const;

    bool selected() const;
    void setSelected(bool sel);

signals:
    void renderDataChanged(ChangeId_t);
    void selectedStatusChanged(bool);

public slots:

protected slots:
    void sendRenderDataChange();

private:
    void manageParentConnections(MapComponent* pOld, MapComponent* pNew);

    bool m_bSelected;
};

MODEL_END_NAMESPACE

#endif // MAPSUBCOMPONENT_H
