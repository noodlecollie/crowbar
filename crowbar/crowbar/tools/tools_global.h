#ifndef TOOLS_GLOBAL_H
#define TOOLS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOOLS_LIBRARY)
#  define TOOLSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOOLSSHARED_EXPORT Q_DECL_IMPORT
#endif

#define TOOLS_NAMESPACE Tools
#define TOOLS_BEGIN_NAMESPACE namespace TOOLS_NAMESPACE {
#define TOOLS_END_NAMESPACE }

#endif // TOOLS_GLOBAL_H
