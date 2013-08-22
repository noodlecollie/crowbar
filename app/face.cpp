#include "face.h"

// Does NOT check for edge existence.
void Face3D::addEdge(GEOMHANDLE edge)
{
    m_hEdges->append(edge);
}

bool Face3D::containsEdge(const GEOMHANDLE edge) const
{
    for ( int i = 0; i < m_hEdges->size(); i++ )
    {
        if ( m_hEdges->at(i) == edge ) return true;
    }
    
    return false;
}

bool Face3D::containsAnyEdge(const QList<GEOMHANDLE> &edges) const
{
    for ( int i = 0; i < m_hEdges->size(); i++ )
    {
        GEOMHANDLE edge = m_hEdges->at(i);
        
        foreach(GEOMHANDLE h, edges)
        {
            if ( edge == h )
            {
                return true;
            }
        }
    }
    
    return false;
}
