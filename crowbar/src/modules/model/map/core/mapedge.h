#ifndef MAPEDGE_H
#define MAPEDGE_H

#include "model_global.h"
#include "maphandleobject.h"
#include "mapvertex.h"
#include <QPointer>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapEdge : public MapHandleObject
{
    Q_OBJECT
    Q_PROPERTY(MapVertex* vertex0 READ vertex0 WRITE setVertex0 NOTIFY vertex0Changed)
    Q_PROPERTY(MapVertex* vertex1 READ vertex1 WRITE setVertex1 NOTIFY vertex1Changed)
public:
    explicit MapEdge(QObject *parent = 0);
    virtual ~MapEdge();

    MapVertex* vertex0() const;
    void setVertex0(MapVertex* v0);

    MapVertex* vertex1() const;
    void setVertex1(MapVertex* v1);

signals:
    void vertex0Changed(MapVertex*);
    void vertex1Changed(MapVertex*);

public slots:

private:
    QPointer<MapVertex> m_pVertex0;
    QPointer<MapVertex> m_pVertex1;
};

MODEL_END_NAMESPACE

#endif // MAPEDGE_H
