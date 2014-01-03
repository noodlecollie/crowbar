#ifndef BRUSHVERTEX_H
#define BRUSHVERTEX_H

#include "geometry_global.h"

#include <QVector3D>
#include <QColor>
#include <QQuaternion>
#include "geommetahandle.h"
#include "ibasictransform.h"

GEOMETRY_BEGIN_NAMESPACE

class GEOMETRYSHARED_EXPORT BrushVertex : public GeomMetaHandle, public IBasicTransform
{
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    BrushVertex(QObject *parent = 0);       // Default constructor.
    BrushVertex(const BrushVertex &other);  // Copy constructor
    
    // Constructor specifying co-ordinates and colour.
    BrushVertex(const QVector3D &position,
                const QColor &col = QColor(255,255,255,255),
                QObject *parent = 0);
    
    // Position property
    QVector3D position() const;
    void setPosition(const QVector3D &pos);
    
    // Colour property
    QColor color() const;
    void setColor(const QColor &col);
    
    // GeomMetaHandle virtual functions
    virtual bool isValid() const;
    virtual QBox3D boundingBox() const;
    virtual QVector3D center() const;
    
    // IBasicTransform virtual functions
    virtual void translate(const QVector3D delta);
    virtual void rotate(const QVector3D origin, const QQuaternion rot);
    
signals:
    void positionChanged(QVector3D newPosition);
    void colorChanged(QColor newColor);
    
private:
    QVector3D   m_vecPosition;  // Position in 3D space.
    QColor      m_colColour;    // Render colour of the vertex.
};

GEOMETRY_END_NAMESPACE

#endif // BRUSHVERTEX_H
