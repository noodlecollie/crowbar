/** \file iconsole_global.h
 * \brief Defines some global properties for the library.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef ICONSOLE_GLOBAL_H
#define ICONSOLE_GLOBAL_H

#include <QtCore/qglobal.h>

//! \def ICONSOLESHARED_EXPORT
//! \brief DLL import/export prefix.

#if defined(ICONSOLE_LIBRARY)
#  define ICONSOLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ICONSOLESHARED_EXPORT Q_DECL_IMPORT
#endif

//! \def ICONSOLE_BEGIN_NAMESPACE
//! \brief Macro for beginning the IConsole namespace.

//! \def ICONSOLE_END_NAMESPACE
//! \brief Macro for ending the IConsole namespace.

#define ICONSOLE_BEGIN_NAMESPACE namespace NIConsole {
#define ICONSOLE_END_NAMESPACE }

#endif // ICONSOLE_GLOBAL_H

/**@}*/
