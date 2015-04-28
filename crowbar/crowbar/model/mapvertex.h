#ifndef MAPVERTEX_H
#define MAPVERTEX_H

#include "positionalsubcomponent.h"
#include "model_global.h"
#include <QVector3D>
#include <QColor>
#include "shared_vectors.h"
#include <QPointer>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapVertex : public PositionalSubComponent
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    explicit MapVertex(MapComponent* parent = 0);
    explicit MapVertex(const QVector3D &position, const QColor &colour, MapComponent* parent = 0);
    explicit MapVertex(const QVector3D &position, MapComponent* parent = 0);

    QVector3D position() const;
    void setPosition(const QVector3D &pos);

    QColor color() const;
    void setColor(const QColor &col);

    virtual QBox3D boundingBox() const;
    virtual QVector3D centroid() const;

signals:
    void positionChanged(const QVector3D&);
    void colorChanged(const QColor&);

public slots:

private:
    QVector3D   m_vecPosition;
    QColor      m_colColour;
};

namespace Vectors
{
    class MODELSHARED_EXPORT ExposedPVector_MapVertex : public QObject
    {
        Q_OBJECT
        EXPOSED_POINTER_VECTOR_PROPS(MapVertex)
    signals:
        void itemAdded(MapVertex*);
        void itemRemoved(MapVertex*);
    };

    class MODELSHARED_EXPORT ExposedOVector_MapVertex : public QObject
    {
        Q_OBJECT
        EXPOSED_OWNER_VECTOR_PROPS(MapVertex)
    signals:
        void itemAdded(MapVertex*);
        void itemRemoved(MapVertex*);
    };

    class MODELSHARED_EXPORT ExposedQPVector_MapVertex : public QObject
    {
        Q_OBJECT
        EXPOSED_QPOINTER_VECTOR_PROPS(MapVertex)
    signals:
        void itemAdded(MapVertex*);
        void itemRemoved(MapVertex*);
    };
}

MODEL_END_NAMESPACE

#endif // MAPVERTEX_H
