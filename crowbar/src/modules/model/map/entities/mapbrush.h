#ifndef MAPBRUSH_H
#define MAPBRUSH_H

#include "model_global.h"
#include "maphandleobject.h"
#include <QVector>
#include "mapvertex.h"
#include "mapedge.h"
#include "mapface.h"
#include "helper_macros.h"

MODEL_BEGIN_NAMESPACE

class MapBrush : public MapHandleObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<MapVertex> vertices READ vertices)
    DECLARE_LIST_TYPE(MapVertex*, vertices)
    DECLARE_QML_LIST_TYPE(MapVertex, vertices)

    Q_PROPERTY(QQmlListProperty<MapEdge> edges READ edges)
    DECLARE_LIST_TYPE(MapEdge*, edges)
    DECLARE_QML_LIST_TYPE(MapEdge, edges)

    Q_PROPERTY(QQmlListProperty<MapFace> faces READ faces)
    DECLARE_LIST_TYPE(MapFace*, faces)
    DECLARE_QML_LIST_TYPE(MapFace, faces)
public:
    explicit MapBrush(QObject *parent = 0);
    virtual ~MapBrush();

signals:
    DECLARE_LIST_TYPE_SIGNALS(MapVertex*, vertices)
    DECLARE_LIST_TYPE_SIGNALS(MapEdge*, edges)
    DECLARE_LIST_TYPE_SIGNALS(MapFace*, faces)

public slots:

private:
};

MODEL_END_NAMESPACE

#endif // MAPBRUSH_H
