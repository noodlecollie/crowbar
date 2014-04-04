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

//! \def GEOMETRYSHARED_EXPORT
//! \brief DLL import/export prefix.

#if defined(GEOMETRY_LIBRARY)
#  define GEOMETRYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GEOMETRYSHARED_EXPORT Q_DECL_IMPORT
#endif

//! \def GEOMETRY_BEGIN_NAMESPACE
//! \brief Macro for beginning the Geometry namespace.

//! \def GEOMETRY_END_NAMESPACE
//! \brief Macro for ending the Geometry namespace.

#define GEOMETRY_NAMESPACE NGeometry
#define GEOMETRY_BEGIN_NAMESPACE namespace GEOMETRY_NAMESPACE {
#define GEOMETRY_END_NAMESPACE }

#endif // GEOMETRY_GLOBAL_H

/**@}*/
