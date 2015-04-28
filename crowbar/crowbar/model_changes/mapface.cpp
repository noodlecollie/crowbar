#include "mapface.h"
#include "mapvertex.h"
#include "mapdocument.h"

MODEL_BEGIN_NAMESPACE

MapFace::MapFace(QObject *parent) :
    MapComponent(parent), m_Vertices(), m_pDocument(NULL)
{
}

MapFace::MapFace(MapVertex **vertices, int count, QObject *parent) :
    MapComponent(parent), m_Vertices(), m_pDocument(NULL)
{
    for ( int i = 0; i < count; i++ )
    {
        addVertex(vertices[i]);
    }
}

MapFace::MapFace(const MapVertexList &list, QObject *parent) :
    MapComponent(parent), m_Vertices(), m_pDocument(NULL)
{
    for ( MapVertexList::const_iterator it = list.cbegin(); it != list.cend(); ++it  )
    {
        addVertex(*it);
    }
}

void MapFace::addVertex(MapVertex *vertex)
{
    if (!vertex) return;

    m_Vertices.append(vertex);
    connect(vertex, SIGNAL(vertexRenderDataChanged()), this, SIGNAL(faceRenderDataChanged()));
    emit faceRenderDataChanged();
}

void MapFace::removeVertex(MapVertex *vertex)
{
    int index = m_Vertices.indexOf(vertex);
    if ( index > 0 )
    {
        disconnect(vertex, SIGNAL(vertexRenderDataChanged()), this, SIGNAL(faceRenderDataChanged()));
        m_Vertices.remove(index);
        emit faceRenderDataChanged();
    }
}

MapVertex* MapFace::removeVertexAt(int index)
{
    MapVertex* v = m_Vertices.at(index);
    disconnect(v, SIGNAL(vertexRenderDataChanged()), this, SIGNAL(faceRenderDataChanged()));
    m_Vertices.removeAt(index);
    emit faceRenderDataChanged();
    return v;
}

int MapFace::indexOfVertex(MapVertex *vertex) const
{
    return m_Vertices.indexOf(vertex);
}

int MapFace::vertexCount() const
{
    return m_Vertices.count();
}

bool MapFace::containsVertex(MapVertex *vertex) const
{
    return m_Vertices.contains(vertex);
}

void MapFace::clearVertices()
{
    foreach(MapVertex* v, m_Vertices)
    {
        disconnect(v, SIGNAL(vertexRenderDataChanged()), this, SIGNAL(faceRenderDataChanged()));
    }

    m_Vertices.clear();
    emit faceRenderDataChanged();
}

QVector3D MapFace::normal() const
{
    if ( vertexCount() < 3 ) return QVector3D();
    
    return QVector3D::normal(m_Vertices.at(0)->position(), m_Vertices.at(1)->position(), m_Vertices.at(2)->position());
}

MapVertex* MapFace::vertexAt(int index)
{
    return m_Vertices.at(index);
}

const MapVertex* MapFace::vertexAt(int index) const
{
    return m_Vertices.at(index);
}

MapDocument* MapFace::document()
{
    return m_pDocument;
}

const MapDocument* MapFace::document() const
{
    return m_pDocument;
}

void MapFace::setDocument(MapDocument *doc)
{
    m_pDocument = doc;
}

MODEL_END_NAMESPACE
