#include "handlepool.h"

MODEL_BEGIN_NAMESPACE

HandlePool::HandlePool()
{
}

void HandlePool::reset()
{
    m_iHandleCounter = INVALID_HANDLE;
}

MapHandle_t HandlePool::assignNextHandle()
{
    MapHandle_t next = peekNextHandle();
    if ( next != INVALID_HANDLE ) m_iHandleCounter = next;
    return next;
}

MapHandle_t HandlePool::peekNextHandle() const
{
    // INVALID_HANDLE means none assigned yet.
    if ( m_iHandleCounter == INVALID_HANDLE ) return 0;

    // Some handles have been assigned but not the max.
    if ( m_iHandleCounter >= 0 && m_iHandleCounter < MAX_HANDLE)
        return m_iHandleCounter + 1;

    // The max handle has been assigned.
    return INVALID_HANDLE;
}

MODEL_END_NAMESPACE

