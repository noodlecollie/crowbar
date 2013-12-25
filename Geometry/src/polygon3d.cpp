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
    
    QList<Edge3D*> Polygon3D::edges() const
    {
        QList list;
        
        for ( int i = 0; i < 3; i++ )
        {
            list.append(m_pEdges[i]);
        }
        
        return list;
    }
    
    // Sets a maximum of 3 edges.
    void Polygon3D::setEdges(QList<Edge3D *> &list)
    {
        for ( int i = 0; i < list.count() && i < 3; i++ )
        {
            m_pEdges[i] = list.at(i);
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
        // The most difficult bit is writing this so that it's readable.
        
        // Create a temporary struct to specify which vertex (0 or 1) from edge 0 is contained at which position (0 or 1) in edge 1 or 2.
        struct EdgeLink
        {
            int containsEdge0Vertex;
            int atPosition;
        };
        
        // Struct to hold this information for edge 1.
        EdgeLink edge1;
        edge1.containsEdge0Vertex = -1;
        edge1.atPosition = -1;
        
        // Check whether edge 1 contains either vertex of edge 0.
        // If it doesn't, the loop is not closed.
        int v = edges[1]->containsVertex(edges[0]->getVertex(0));
        
        // The edge contained vertex 0.
        if ( v >= 0 )
        {
            edge1.containsEdge0Vertex = 0;
            edge1.atPosition = v;
        }
        
        // The edge did not contain vertex 0.
        else
        {
            v = edges[1]->containsVertex(edges[0]->getVertex(1));
            
            // The edge contained vertex 1.
            if ( v >= 0 )
            {
                edge1.containsEdge0Vertex = 1;
                edge1.atPosition = v;
            }
            
            // The edge contained neither vertex - there is no closed loop.
            else
            {
                return false;
            }
        }
        
        // Edge 1 contained either vertex 0 or 1 of edge 0.
        // If it contained vertex 0 then edge 2 -should- contain vertex 1, and vice versa.
        // Record this here.
        EdgeLink edge2;
        edge2.containsEdge0Vertex = (edge1.containsEdge0Vertex == 0) ? 1 : 0;
        
        // Check whether edge 2 does actually contain this vertex. If it doesn't, there is no closed loop.
        edge2.atPosition = edges[2]->containsVertex(edges[0]->getVertex(edge2.containsEdge0Vertex));
        if ( edge2.atPosition < 0 )
        {
            return false;
        }
        
        // Record the indices of the vertices that edges 1 and 2 should have in common.
        int edge1Common = (edge1.atPosition == 0) ? 1 : 0;  // Common indices will be in the opposite slot
        int edge2Common = (edge2.atPosition == 0) ? 1 : 0;  // to the slot that holds the edge 0 vertex.
        
        // If these vertices are in common, the edges form a closed loop.
        // If not, there is no closed loop.
        return edges[1]->getVertex(edge1Common) == edges[2]->getVertex(edge2Common);
    }
}
