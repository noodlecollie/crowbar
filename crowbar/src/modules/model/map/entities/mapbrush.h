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

class MODELSHARED_EXPORT MapBrush : public MapHandleObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<MODEL_NAMESPACE::MapVertex> vertices READ vertices)
    DECLARE_LIST_TYPE(MODEL_NAMESPACE::MapVertex*, vertices)
    DECLARE_QML_LIST_TYPE(MODEL_NAMESPACE::MapVertex, vertices)

    Q_PROPERTY(QQmlListProperty<MODEL_NAMESPACE::MapEdge> edges READ edges)
    DECLARE_LIST_TYPE(MODEL_NAMESPACE::MapEdge*, edges)
    DECLARE_QML_LIST_TYPE(MODEL_NAMESPACE::MapEdge, edges)

    Q_PROPERTY(QQmlListProperty<MODEL_NAMESPACE::MapFace> faces READ faces)
    DECLARE_LIST_TYPE(MODEL_NAMESPACE::MapFace*, faces)
    DECLARE_QML_LIST_TYPE(MODEL_NAMESPACE::MapFace, faces)
public:
    explicit MapBrush(QObject *parent = 0);
    virtual ~MapBrush();

signals:
    DECLARE_LIST_TYPE_SIGNALS(MODEL_NAMESPACE::MapVertex*, vertices)
    DECLARE_LIST_TYPE_SIGNALS(MODEL_NAMESPACE::MapEdge*, edges)
    DECLARE_LIST_TYPE_SIGNALS(MODEL_NAMESPACE::MapFace*, faces)

public slots:

private:
};

MODEL_END_NAMESPACE

#endif // MAPBRUSH_H
