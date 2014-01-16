#ifndef OCTREE_GLOBAL_H
#define OCTREE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OCTREE_LIBRARY)
#  define OCTREESHARED_EXPORT Q_DECL_EXPORT
#else
#  define OCTREESHARED_EXPORT Q_DECL_IMPORT
#endif

#define OCTREE_BEGIN_NAMESPACE namespace NOctree {
#define OCTREE_END_NAMESPACE }

#endif // OCTREE_GLOBAL_H
