#ifndef IMAPHANDLE_H
#define IMAPHANDLE_H

#include "model_global.h"
#include <QtDebug>

#define MAX_HANDLE      2147483647
#define INVALID_HANDLE  -1

typedef int MapHandle_t;

MODEL_BEGIN_NAMESPACE

class IMapHandle
{
public:
    virtual ~IMapHandle() {}
    
    virtual void setHandle(MapHandle_t handle) = 0;
    virtual MapHandle_t handle() const = 0;
};

MODEL_END_NAMESPACE

#endif // IMAPHANDLE_H
