#ifndef MAPEDGE_H
#define MAPEDGE_H

#include "positionalsubcomponent.h"
#include "model_global.h"
#include <QPointer>
#include "shared_vectors.h"

MODEL_BEGIN_NAMESPACE

class MapVertex;

class MODELSHARED_EXPORT MapEdge : public PositionalSubComponent
{
    Q_OBJECT
    Q_PROPERTY(MapVertex* vertex0 READ vertex0 WRITE setVertex0 NOTIFY vertex0Changed)
    Q_PROPERTY(MapVertex* vertex1 READ vertex1 WRITE setVertex1 NOTIFY vertex1Changed)
public:
    explicit MapEdge(MapComponent *parent = 0);
    explicit MapEdge(MapVertex* v0, MapVertex* v1, MapComponent *parent = 0);

    void setVertex0(MapVertex* vertex);
    void setVertex1(MapVertex* vertex);
    void setVertex(int index, MapVertex* vertex);
    MapVertex* vertex0();
    const MapVertex* vertex0() const;
    MapVertex* vertex1();
    const MapVertex* vertex1() const;

    virtual QBox3D boundingBox() const;
    virtual QVector3D centroid() const;
    bool isValid() const;

signals:
    void vertex0Changed(MapVertex*);
    void vertex1Changed(MapVertex*);

public slots:

private:
    void manageChangeSignals(MapVertex* vOld, MapVertex* vNew);

    QPointer<MapVertex>  m_pVertex0;
    QPointer<MapVertex>  m_pVertex1;
};

namespace Vectors
{
    class MODELSHARED_EXPORT ExposedPVector_MapEdge : public QObject
    {
        Q_OBJECT
        EXPOSED_POINTER_VECTOR_PROPS(MapEdge)
    signals:
        void itemAdded(MapEdge*);
        void itemRemoved(MapEdge*);
    };

    class MODELSHARED_EXPORT ExposedOVector_MapEdge : public QObject
    {
        Q_OBJECT
        EXPOSED_OWNER_VECTOR_PROPS(MapEdge)
    signals:
        void itemAdded(MapEdge*);
        void itemRemoved(MapEdge*);
    };

    class MODELSHARED_EXPORT ExposedQPVector_MapEdge : public QObject
    {
        Q_OBJECT
        EXPOSED_QPOINTER_VECTOR_PROPS(MapEdge)
    signals:
        void itemAdded(MapEdge*);
        void itemRemoved(MapEdge*);
    };
}

MODEL_END_NAMESPACE

#endif // MAPEDGE_H
