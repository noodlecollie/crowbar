#ifndef DATASTRUCTURES_GLOBAL_H
#define DATASTRUCTURES_GLOBAL_H

#include <QtCore/qglobal.h>

//! \def DATASTRUCTURESSHARED_EXPORT
//! \brief DLL import/export prefix.

#if defined(DATASTRUCTURES_LIBRARY)
#  define DATASTRUCTURESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATASTRUCTURESSHARED_EXPORT Q_DECL_IMPORT
#endif

//! \def DATASTRUCTURES_NAMESPACE
//! \brief Macro for the DataStructures namespace.

//! \def DATASTRUCTURES_BEGIN_NAMESPACE
//! \brief Macro for beginning the DataStructures namespace.

//! \def DATASTRUCTURES_END_NAMESPACE
//! \brief Macro for ending the DataStructures namespace.

#define DATASTRUCTURES_NAMESPACE NDataStructures
#define DATASTRUCTURES_BEGIN_NAMESPACE namespace DATASTRUCTURES_NAMESPACE {
#define DATASTRUCTURES_END_NAMESPACE }

#endif // DATASTRUCTURES_GLOBAL_H
