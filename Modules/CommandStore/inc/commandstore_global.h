/** \file regexutil_global.h
 * \brief Defines some global properties for the library.
 */

/**
 * \defgroup CommandStore CommandStore library
 * @{
 */

#ifndef COMMANDSTORE_GLOBAL_H
#define COMMANDSTORE_GLOBAL_H

#include <QtCore/qglobal.h>

//! \def COMMANDSTORESHARED_EXPORT
//! \brief DLL import/export prefix.

#if defined(COMMANDSTORE_LIBRARY)
#  define COMMANDSTORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMMANDSTORESHARED_EXPORT Q_DECL_IMPORT
#endif

//! \def COMMANDSTORE_BEGIN_NAMESPACE
//! \brief Macro for beginning the CommandStore namespace.

//! \def COMMANDSTORE_END_NAMESPACE
//! \brief Macro for ending the CommandStore namespace.

#define COMMANDSTORE_BEGIN_NAMESPACE namespace NCommandStore {
#define COMMANDSTORE_END_NAMESPACE }

#endif // COMMANDSTORE_GLOBAL_H

/**@}*/
