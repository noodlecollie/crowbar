#ifndef MAPFACE_H
#define MAPFACE_H

#include "model_global.h"
#include "maphandleobject.h"
#include "helper_macros.h"
#include <QVector>
#include <QQmlListProperty>
#include <QVector3D>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapFace : public MapHandleObject
{
    Q_OBJECT

    DECLARE_LIST_TYPE(int, vertices)
    Q_PROPERTY(QVariant vertices READ vertices WRITE setVertices NOTIFY verticesChanged)

    DECLARE_LIST_TYPE(int, edges)
    Q_PROPERTY(QVariant edges READ edges WRITE setEdges NOTIFY edgesChanged)
public:
    explicit MapFace(QObject *parent = 0);
    virtual ~MapFace();

    QVector3D normal() const;
    bool isValid() const;

    QVariant vertices() const;
    void setVertices(const QVariant &list);

    QVariant edges() const;
    void setEdges(const QVariant &list);

signals:
    DECLARE_LIST_TYPE_SIGNALS(int, vertices)
    void verticesChanged();

    DECLARE_LIST_TYPE_SIGNALS(int, edges)
    void edgesChanged();

public slots:

private:
};

#endif // MAPFACE_H

MODEL_END_NAMESPACE
