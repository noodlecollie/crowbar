/** \file regexutil_global.h
 * \brief Defines some global properties for the library.
 */

/**
 * \defgroup RegexUtil RegexUtil library
 * @{
 */

#ifndef REGEXUTIL_GLOBAL_H
#define REGEXUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

//! \def REGEXUTILSHARED_EXPORT
//! \brief DLL import/export prefix.

#if defined(REGEXUTIL_LIBRARY)
#  define REGEXUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define REGEXUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // REGEXUTIL_GLOBAL_H

/**@}*/
