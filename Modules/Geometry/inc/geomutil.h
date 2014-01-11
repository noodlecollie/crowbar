#ifndef GEOMUTIL_H
#define GEOMUTIL_H

#include "geometry_global.h"

class QVector3D;
class QVector2D;

GEOMETRY_BEGIN_NAMESPACE

extern const GEOMETRYSHARED_EXPORT QVector2D    VEC2_ORIGIN;
extern const GEOMETRYSHARED_EXPORT QVector3D    VEC3_ORIGIN;

// Returns the normal of three vertices.
// If the three vertices are specified with a counter-clockwise winding, the
// normal points up at the viewpoint.
QVector3D ccwNormal(const QVector3D &v0, const QVector3D &v1, const QVector3D &v2);

// Calculates the scalar projection of v0 along v1.
qreal scalarProjection(const QVector3D &v0, const QVector3D &v1);

// Calculates the vector projection of v0 along v1.
QVector3D vectorProjection(const QVector3D &v0, const QVector3D &v1);

GEOMETRY_END_NAMESPACE

#endif // GEOMUTIL_H
