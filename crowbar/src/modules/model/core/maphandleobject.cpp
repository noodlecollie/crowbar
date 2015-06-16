#include "maphandleobject.h"

MODEL_BEGIN_NAMESPACE

const int MapHandleObject::INVALID_HANDLE = -1;

MapHandleObject::MapHandleObject(QObject *parent) : ChangeNotifier(parent)
{
    m_iHandle = INVALID_HANDLE;
}

MapHandleObject::~MapHandleObject()
{

}

int MapHandleObject::handle() const
{
    return m_iHandle;
}

void MapHandleObject::setHandle(int handle)
{
    if ( handle == m_iHandle ) return;

    m_iHandle = handle;
    emit handleChanged(m_iHandle);
}

MODEL_END_NAMESPACE
