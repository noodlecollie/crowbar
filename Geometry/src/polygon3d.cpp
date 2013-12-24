#include "polygon3d.h"

namespace Geometry
{
    Polygon3D::Polygon3D(QObject *parent) : GeomMetaHandle(parent)
    {
        // Set edge pointers to NULL.
        for ( int i = 0; i < 3; i++ )
        {
            m_pEdges[i] = NULL;
        }
    }
    
    virtual bool Polygon3D::isValid() const
    {
        // Ensure edges form a closed loop.
        // Pointers are validated in this function too.
        return closedLoop(m_pEdges);
    }
    
    bool Polygon3D::closedLoop(Edge3D *edges[])
    {
        // All edges must be valid.
        for ( int i = 0; i < 3; i++ )
        {
            if ( !edges[i] || !edges[i]->isValid() ) return false;
        }
        
        // For a closed loop:
        // - One of the vertices in edge 0 must be in edge 1, and the other in edge 2.
        // - The remaining two vertices must be the same.
        
        // Create a temporary struct to specify which edge (1 or 2) contains which vertex (0 or 1) from edge 0.
        struct
        {
            int edgeNo;
            int vertNo;
        } ContainsE0Vertex;
    }
}
