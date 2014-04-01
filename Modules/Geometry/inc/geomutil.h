#ifndef GEOMUTIL_H
#define GEOMUTIL_H

#include "geometry_global.h"

class QVector3D;
class QVector2D;
class QBox3D;

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

// Determines whether two bounding boxes intersect.
// Returns false if they are completely disjoint, true otherwise.
bool boundingBoxesIntersect(const QVector3D &max1, const QVector3D &min1, const QVector3D &max2, const QVector3D &min2);
bool boundingBoxesIntersect(const QBox3D &box1, const QBox3D &box2);

void removeme();

GEOMETRY_END_NAMESPACE

#endif // GEOMUTIL_H
