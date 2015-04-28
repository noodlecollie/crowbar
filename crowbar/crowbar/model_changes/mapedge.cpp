#include "mapedge.h"
#include "mapdocument.h"

MODEL_BEGIN_NAMESPACE

MapEdge::MapEdge(QObject *parent) :
    MapComponent(parent), m_pVertices(), m_pDocument(NULL)
{
    m_pVertices[0] = NULL;
    m_pVertices[1] = NULL;
}

MapEdge::MapEdge(MapVertex *v0, MapVertex *v1, QObject *parent) :
    MapComponent(parent), m_pVertices(), m_pDocument(NULL)
{
    m_pVertices[0] = v0;
    m_pVertices[1] = v1;
}

void MapEdge::setVertex0(MapVertex *vertex)
{
    m_pVertices[0] = vertex;
    emit vertex0Changed(m_pVertices[0]);
    emit edgeRenderDataChanged();
}

void MapEdge::setVertex1(MapVertex *vertex)
{
    m_pVertices[1] = vertex;
    emit vertex1Changed(m_pVertices[1]);
    emit edgeRenderDataChanged();
}

void MapEdge::setVertex(int index, MapVertex *vertex)
{
    if ( index == 0 ) setVertex0(vertex);
    else if ( index == 1 ) setVertex1(vertex);
}

MapVertex* MapEdge::vertex0()
{
    return m_pVertices[0];
}

const MapVertex* MapEdge::vertex0() const
{
    return m_pVertices[0];
}

MapVertex* MapEdge::vertex1()
{
    return m_pVertices[1];
}

const MapVertex* MapEdge::vertex1() const
{
    return m_pVertices[1];
}

MapDocument* MapEdge::document()
{
    return m_pDocument;
}

const MapDocument* MapEdge::document() const
{
    return m_pDocument;
}

void MapEdge::setDocument(MapDocument *doc)
{
    m_pDocument = doc;
}

MODEL_END_NAMESPACE
