#ifndef FRUSTUM_UTIL_H
#define FRUSTUM_UTIL_H

#include "geometry_global.h"

#include <Qt3D/QPlane3D>

GEOMETRY_BEGIN_NAMESPACE

// Group of planes that represent a frustum. Normals should point out from the viewing volume.
struct Frustum
{
    // Near plane and far plane should be parallel to each other.
    QPlane3D    nearPlane;
    QPlane3D    farPlane;
    
    QPlane3D    leftPlane;
    QPlane3D    rightPlane;
    QPlane3D    topPlane;       // Should be in the general direction of the camera's up vector.
    QPlane3D    bottomPlane;
};

GEOMETRY_END_NAMESPACE

#endif // FRUSTUM_UTIL_H
