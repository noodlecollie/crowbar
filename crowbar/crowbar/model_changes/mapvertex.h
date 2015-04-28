#ifndef MAPVERTEX_H
#define MAPVERTEX_H

#include "model_global.h"
#include "mapcomponent.h"
#include "imaphandle.h"
#include <QVector3D>
#include <QColor>

MODEL_BEGIN_NAMESPACE

class MapDocument;

class MODELSHARED_EXPORT MapVertex : public MapComponent
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    explicit MapVertex(QObject *parent = 0);
    explicit MapVertex(const QVector3D &pos, QObject* parent = 0);
    explicit MapVertex(const QVector3D &pos, const QColor &col, QObject* parent = 0);
    
public: // IMapHandle
    void setPosition(const QVector3D &position);
    QVector3D position() const;
    
    void setColor(const QColor &colour);
    QColor color() const;

    MapDocument* document();
    const MapDocument* document() const;
    void setDocument(MapDocument* doc);
    
signals:
    void positionChanged(const QVector3D&);
    void colorChanged(const QColor&);
    void vertexRenderDataChanged();
public slots:
    
private:
    QVector3D       m_vecPosition;
    QColor          m_colColour;
    MapDocument*    m_pDocument;
};

typedef QList<MapVertex*> MapVertexList;

MODEL_END_NAMESPACE

#endif // MAPVERTEX_H
