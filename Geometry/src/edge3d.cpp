#include "edge3d.h"

namespace Geometry
{
    Edge3D::Edge3D(QObject *parent) : GeomMetaHandle(parent),
        m_pVertexA(NULL), m_pVertexB(NULL)
    {
    }
    
    Edge3D::Edge3D(Vertex3D *vertA, Vertex3D *vertB, QObject *parent) : GeomMetaHandle(parent),
        m_pVertexA(vertA), m_pVertexB(vertB)
    { 
    }
    
    Edge3D::Edge3D(const QPair<Vertex3D *, Vertex3D *> &pair, QObject *parent) : GeomMetaHandle(parent),
        m_pVertexA(pair.first), m_pVertexB(pair.second)
    {
    }
    
    void Edge3D::setVertexA(Vertex3D* vertA)
    {
        m_pVertexA = vertA;
    }
    
    Vertex3D* Edge3D::vertexA() const
    {
        return m_pVertexA;
    }
    
    void Edge3D::setVertexB(Vertex3D *vertB)
    {
        m_pVertexB = vertB;
    }
    
    Vertex3D* Edge3D::vertexB() const
    {
        return m_pVertexB;
    }
    
    void Edge3D::setVertex(int n, Vertex3D *vert)
    {
        if ( n == 0 ) m_pVertexA = vert;
        else if ( n == 1 ) m_pVertexB = vert;
    }
    
    Vertex3D* Edge3D::getVertex(int n) const
    {
        if ( n == 0 ) return m_pVertexA;
        else if ( n == 1 ) return m_pVertexB;
        else return NULL;
    }
    
    void Edge3D::setVertex(const QPair<Vertex3D *, Vertex3D *> &pair)
    {
        m_pVertexA = pair.first;
        m_pVertexB = pair.second;
    }
    
    QPair<Vertex3D*, Vertex3D*> Edge3D::getVertex() const
    {
        return QPair<Vertex3D*, Vertex3D*>(m_pVertexA, m_pVertexB);
    }
    
    int Edge3D::containsVertex(Vertex3D *vert)
    {
        if ( vert == m_pVertexA ) return 0;
        else if ( vert == m_pVertexB ) return 1;
        else return -1;
    }
    
    bool Edge3D::hasMidpoint() const
    {
        return isValid();
    }
    
    QVector3D Edge3D::midpoint(bool *success) const
    {
        // If has no midpoint, return false with a zero vector.
        if ( !hasMidpoint() )
        {
            *success = false;
            return VEC3_ORIGIN;
        }
        
        *success = true;
        
        // Midpoint is half of the vector between the two vertices.
        // This must be added to one of the vertices as a start point.
        QVector3D p1 = m_pVertexA->position();
        QVector3D p2 = m_pVertexB->position();
        QVector3D dir = p2 - p1;
        
        return p1 + (0.5 * dir);
    }
    
    virtual bool Edge3D::isValid() const
    {
        // Both vertex pointers must be valid.
        return m_pVertexA && m_pVertexB;
    }
}
