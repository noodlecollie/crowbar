/** \file frustum_util.h
 * \brief Defines useful utilities related to the 3D camera frustum.
 */

#ifndef FRUSTUM_UTIL_H
#define FRUSTUM_UTIL_H

#include "geometry_global.h"

#include <Qt3D/QPlane3D>

GEOMETRY_BEGIN_NAMESPACE

// Group of planes that represent a frustum. Normals should point out from the viewing volume.
/**
 * @brief Struct to hold a group of planes that represent the camera frustum.
 * @note Normals should point out from the viewing volume.
 */
struct Frustum
{
    // Near plane and far plane should be parallel to each other.
    QPlane3D    nearPlane;      /**< Near clip plane. */
    QPlane3D    farPlane;       /**< Far clip plane. */
    
    QPlane3D    leftPlane;      /**< Left clip plane. */
    QPlane3D    rightPlane;     /**< Right clip plane. */
    QPlane3D    topPlane;       /**< Up clip plane (relative to camera). */
    QPlane3D    bottomPlane;    /**< Down clip plane. */
};

GEOMETRY_END_NAMESPACE

#endif // FRUSTUM_UTIL_H
