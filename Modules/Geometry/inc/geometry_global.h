/** \file geometry_global.h
 * \brief Defines some global properties for the library.
 */

/**
 * \defgroup Geometry Geometry library
 * @{
 */

#ifndef GEOMETRY_GLOBAL_H
#define GEOMETRY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GEOMETRY_LIBRARY)
#  define GEOMETRYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GEOMETRYSHARED_EXPORT Q_DECL_IMPORT
#endif

#define GEOMETRY_BEGIN_NAMESPACE namespace NGeometry {
#define GEOMETRY_END_NAMESPACE }

#endif // GEOMETRY_GLOBAL_H

/**@}*/
