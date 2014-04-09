#ifndef IBRUSHOPS_H
#define IBRUSHOPS_H

#include "geometry_global.h"

#include <Qt3D/QPlane3D>

GEOMETRY_BEGIN_NAMESPACE

class Brush3D;

class IBrushOps
{
public:
    virtual ~IBrushOps() {}
    // Splits the brush. Positive is the section in front of the plane (in the direction of the normal),
    // negative is the section in the opposite direction.
    // Positive and negative should both be empty brushes.
    // If the return is false, the plane did not split the brush and both positive and negative remain unmodified.
    virtual bool split(const QPlane3D plane, Brush3D &positive, Brush3D &negative) = 0;

};

GEOMETRY_END_NAMESPACE

#endif // IBRUSHOPS_H
