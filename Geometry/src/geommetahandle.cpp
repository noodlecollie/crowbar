#include "geommetahandle.h"

namespace Geometry
{
    GeomMetaHandle::GeomMetaHandle(QObject *parent) :
        QObject(parent), m_bSelected(false), m_hID(INVALID_HANDLE)
    {
    }
    
    void GeomMetaHandle::setSelected(bool b)
    {
        m_bSelected = b;
    }
    
    bool GeomMetaHandle::selected() const
    {
        return m_bSelected;
    }
    
    GEOMHANDLE GeomMetaHandle::id() const
    {
        return m_hID;
    }
    
    void GeomMetaHandle::setId(GEOMHANDLE id)
    {
        m_hID = id;
    }
}
