#ifndef HANDLEPOOL_H
#define HANDLEPOOL_H

#include "model_global.h"
#include "imaphandle.h"

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT HandlePool
{
public:
    HandlePool();

    // Returns the next available handle in the pool without assigning it.
    MapHandle_t peekNextHandle() const;

    // Returns the next available handle and increments the handle counter,
    // meaning the returned handle is now "assigned" to the caller.
    // If no more handles are available, INVALID_HANDLE is returned.
    MapHandle_t assignNextHandle();

    // Resets the handle counter, essentially freeing all assigned handles.
    void reset();

private:
    MapHandle_t m_iHandleCounter;
};

MODEL_END_NAMESPACE

#endif // HANDLEPOOL_H
