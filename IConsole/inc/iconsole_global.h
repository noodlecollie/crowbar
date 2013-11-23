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

#endif // ICONSOLE_GLOBAL_H

/**@}*/
