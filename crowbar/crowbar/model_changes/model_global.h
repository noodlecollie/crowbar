#ifndef MODEL_GLOBAL_H
#define MODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MODEL_LIBRARY)
#  define MODELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODELSHARED_EXPORT Q_DECL_IMPORT
#endif

#define MODEL_NAMESPACE Model
#define MODEL_NAMESPACE_STRING "Model"
#define MODEL_BEGIN_NAMESPACE namespace MODEL_NAMESPACE {
#define MODEL_END_NAMESPACE }

#endif // MODEL_GLOBAL_H
