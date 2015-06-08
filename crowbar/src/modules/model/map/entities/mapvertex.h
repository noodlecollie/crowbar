#ifndef MAPVERTEX_H
#define MAPVERTEX_H

#include "model_global.h"
#include <QObject>
#include <QVector3D>
#include <QColor>
#include <QVector2D>

MODEL_BEGIN_NAMESPACE

class MapVertex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QVector2D textureCoordinate READ textureCoordinate WRITE setTextureCoordinate NOTIFY textureCoordinateChanged)
public:
    explicit MapVertex(QObject *parent = 0);
    virtual ~MapVertex();
    
signals:
    
public slots:
    
private:
    
    // We have the following properties:
    QVector3D   m_vecPosition;  // Position in 3D space.
    QColor      m_colColour;    // Per-vertex colour (overridden by other colours, eg. face).
    QVector2D   m_vecTexCoord;  // Texture co-ordinate.
};

MODEL_END_NAMESPACE

#endif // MAPVERTEX_H
