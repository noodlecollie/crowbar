#include "solid.h"

Solid3D::Solid3D() : m_Vertices(), m_Edges(), m_Faces(),
    m_hVBOHandle(NULLHND), m_IndexPool(), m_hHandle(NULLHND)
{
}

void Solid3D::setHandle(const GEOMHANDLE handle)
{
    m_hHandle = handle;
    
    // Update parent handle in all components.
    
    foreach ( Vertex3D* v, m_Vertices ) v->setParentSolid(m_hHandle);
    
    foreach ( Edge3D* e, m_Edges ) e->setParentSolid(m_hHandle);
    
    foreach ( Face3D* f, m_Faces ) f->setParentSolid(m_hHandle);
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
    Q_ASSERT(vertex);
    // If we've hit the vertex limit, don't add.
    if ( m_Vertices.size() >= MAX_VERTICES ) return false;
    
    // Make sure we can get an index.
    GEOMHANDLE vertHandle = m_IndexPool.allocateIndex();
    //if ( vertHandle == NULLHND ) return false;
    Q_ASSERT(vertHandle > NULLHND);
    
    vertex->setHandle(vertHandle);
    vertex->setParentSolid(getHandle());
    vertex->setVBOHandle(m_Vertices.size());
    m_Vertices.append(vertex);
    return true;
}

Vertex3D* Solid3D::removeVertexHandle(const GEOMHANDLE vertex)
{
    // Find the vertex in the vector.
    for ( int i = 0; i < m_Vertices.size(); i++ )
    {
        if ( m_Vertices.at(i)->getHandle() == vertex )
        {
            // Record the vertex.
            Vertex3D* v = m_Vertices.at(i);
            v->setParentSolid(NULLHND);
            
            // We need to update the VBO handles for the rest of the
            // vertices that will be shifted down.
            for ( int j = i+1; j < m_Vertices.size(); j++ )
            {
                m_Vertices.at(j)->setVBOHandle(j);
            }
            
            // Remove the old vertex.
            m_Vertices.remove(i);
            
            // Return it.
            return v;
        }
    }
    
    // We didn't find the vertex, so return NULL.
    return NULL;
}

Vertex3D* Solid3D::findVertex(const GEOMHANDLE vertex) const
{
    foreach( Vertex3D* v, m_Vertices )
    {
        if ( v->getHandle() == vertex )
        {
            return v;
        }
    }
    
    return NULL;
}

void Solid3D::findRelatedEdgesForVertex(QList<GEOMHANDLE> &outEdges, GEOMHANDLE vertex)
{
    // Search through the edge list and find any edges which reference this vertex.
    foreach(Edge3D* e, m_Edges)
    {
        if ( e->getStartVertex() == vertex || e->getEndVertex() == vertex ) outEdges.append(e->getHandle());
    }
}

// A bit faster than doing multiple searches for individual vertices.
void Solid3D::findRelatedEdgesForVertex(QList<GEOMHANDLE> &outEdges, QList<GEOMHANDLE> &vertices)
{
    // Search through the edge list and find any edges which reference any of these vertices.
    foreach(Edge3D* e, m_Edges)
    {
        foreach(GEOMHANDLE h, vertices)
        {
            if ( e->getStartVertex() == h || e->getEndVertex() == h )
            {
                outEdges.append(e->getHandle());
                break;
            }
        }
    }
}

void Solid3D::findRelatedFacesForVertex(QList<GEOMHANDLE> &outFaces, GEOMHANDLE vertex)
{
    // First find the edges which contain this vertex.
    QList<GEOMHANDLE> edgeList;
    findRelatedEdgesForVertex(edgeList, vertex);
    
    // Then find the faces which contain any of these edges.
    findRelatedFacesForEdge(outFaces, edgeList);
}

void Solid3D::findRelatedFacesForVertex(QList<GEOMHANDLE> &outFaces, QList<GEOMHANDLE> &vertices)
{
    // First find the edges which contain any of these vertices.
    QList<GEOMHANDLE> edgeList;
    findRelatedEdgesForVertex(edgeList, vertices);
    
    // Then find the faces which contain any of these edges.
    findRelatedFacesForEdge(outFaces, edgeList);
}

bool Solid3D::addEdge(Edge3D *edge)
{
    Q_ASSERT(edge);
    // If we've hit the edge limit, don't add.
    if ( m_Edges.size() >= MAX_EDGES ) return false;
    
    // Make sure we can get an index.
    GEOMHANDLE edgeHandle = m_IndexPool.allocateIndex();
    Q_ASSERT(edgeHandle > NULLHND);
    
    edge->setHandle(edgeHandle);
    edge->setParentSolid(getHandle());
    m_Edges.append(edge);
    return true;
}

Edge3D* Solid3D::removeEdgeHandle(const GEOMHANDLE edge)
{
    // Find the edge in the vector.
    for ( int i = 0; i < m_Edges.size(); i++ )
    {
        if ( m_Edges.at(i)->getHandle() == edge )
        {
            // Record the edge.
            Edge3D* e = m_Edges.at(i);
            e->setParentSolid(NULLHND);
            
            // Remove the old edge.
            m_Edges.remove(i);
            
            // Return it.
            return e;
        }
    }
    
    // We didn't find the edge, so return NULL.
    return NULL;
}

Edge3D* Solid3D::findEdge(const GEOMHANDLE edge) const
{
    foreach( Edge3D* e, m_Edges )
    {
        if ( e->getHandle() == edge )
        {
            return e;
        }
    }
    
    return NULL;
}

void Solid3D::findRelatedFacesForEdge(QList<GEOMHANDLE> &outFaces, GEOMHANDLE edge)
{
    // Search through the face list and find any faces which reference this edge.
    foreach(Face3D* f, m_Faces)
    {
        if ( f->containsEdge(edge) ) outFaces.append(f->getHandle());
    }
}

void Solid3D::findRelatedFacesForEdge(QList<GEOMHANDLE> &outFaces, QList<GEOMHANDLE> &edges)
{
    foreach(Face3D* f, m_Faces)
    {
        if ( f->containsAnyEdge(edges) ) outFaces.append(f->getHandle());
    }
}

bool Solid3D::addFace(Face3D *face)
{
    Q_ASSERT(face);
    // If we've hit the face limit, don't add.
    if ( m_Faces.size() >= MAX_FACES ) return false;
    
    // Make sure we can get an index.
    GEOMHANDLE faceHandle = m_IndexPool.allocateIndex();
    Q_ASSERT(faceHandle > NULLHND);
    
    face->setHandle(faceHandle);
    face->setParentSolid(getHandle());
    m_Faces.append(face);
    return true;
}

Face3D* Solid3D::removeFaceHandle(const GEOMHANDLE face)
{
    // Find the face in the vector.
    for ( int i = 0; i < m_Faces.size(); i++ )
    {
        if ( m_Faces.at(i)->getHandle() == face )
        {
            // Record the face.
            Face3D* f = m_Faces.at(i);
            f->setParentSolid(NULLHND);
            
            // Remove the old face.
            m_Faces.remove(i);
            
            // Return it.
            return f;
        }
    }
    
    // We didn't find the face, so return NULL.
    return NULL;
}

Face3D* Solid3D::findFace(const GEOMHANDLE face) const
{
    foreach( Face3D* f, m_Faces )
    {
        if ( f->getHandle() == face )
        {
            return f;
        }
    }
    
    return NULL;
}

void Solid3D::identifyHandle(GEOMHANDLE handle, GeomInfo &info) const
{
    info.m_hHandle = handle;
    
    if ( handle == NULLHND )
    {
        info.m_iType = GeomInfo::Null;
        info.m_hParentSolid = NULLHND;
        return;
    }
    
    // Search vertices.
    Vertex3D* v = findVertex(handle);
    
    if ( v )
    {
        info.m_iType = GeomInfo::Vertex;
        info.m_hParentSolid = v->getParentSolid();
    }
    
    // Search edges.
    Edge3D* e = findEdge(handle);
    
    if ( e )
    {
        info.m_iType = GeomInfo::Edge;
        info.m_hParentSolid = e->getParentSolid();
    }
    
    // Search faces.
    Face3D* f = findFace(handle);
    
    if ( f )
    {
        info.m_iType = GeomInfo::Face;
        info.m_hParentSolid = f->getParentSolid();
    }
}
