#ifndef ICONSOLE_GLOBAL_H
#define ICONSOLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ICONSOLE_LIBRARY)
#  define ICONSOLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ICONSOLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ICONSOLE_GLOBAL_H
