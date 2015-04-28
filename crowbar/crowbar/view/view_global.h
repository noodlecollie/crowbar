#ifndef VIEW_GLOBAL_H
#define VIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VIEW_LIBRARY)
#  define VIEWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VIEWSHARED_EXPORT Q_DECL_IMPORT
#endif

#define VIEW_NAMESPACE View
#define VIEW_NAMESPACE_STRING "View"
#define VIEW_BEGIN_NAMESPACE namespace VIEW_NAMESPACE {
#define VIEW_END_NAMESPACE }

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

#endif // VIEW_GLOBAL_H
