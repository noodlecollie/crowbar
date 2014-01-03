#ifndef IBASICTRANSFORM_H
#define IBASICTRANSFORM_H

#include "geometry_global.h"

#include <QVector3D>
#include <QQuaternion>

GEOMETRY_BEGIN_NAMESPACE

class GEOMETRYSHARED_EXPORT IBasicTransform
{
public:
    virtual ~IBasicTransform() {}
    
    // Translate in the specified X,Y,Z direction.
    virtual void translate(const QVector3D delta) = 0;
    
    // Rotate around the specified point using the specified quaternion.
    virtual void rotate(const QVector3D origin, const QQuaternion rot) = 0;
};

GEOMETRY_END_NAMESPACE

#endif // IBASICTRANSFORM_H
