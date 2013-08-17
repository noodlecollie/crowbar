#include "solid.h"

Solid3D::Solid3D() : m_Vertices(), m_Edges(), m_Faces(), m_iNextHandle(1)
{
}

Solid3D::~Solid3D()
{
    foreach (Face3D* face, m_Faces)
    {
        if ( face ) delete face;
    }
    
    foreach (Edge3D* edge, m_Edges)
    {
        if ( edge ) delete edge;
    }
    
    foreach (Vertex3D* vertex, m_Vertices)
    {
        if ( vertex ) delete vertex;
    }
}

bool Solid3D::addVertex(Vertex3D *vertex)
{
    if ( !canAddVertex() ) return false;
    
    m_Vertices.append(vertex);
    vertex->setOffset(m_Vertices.size());
    vertex->setId(getNextHandle());
}
