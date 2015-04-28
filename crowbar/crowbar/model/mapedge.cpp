#include "mapedge.h"
#include "mapvertex.h"

MODEL_BEGIN_NAMESPACE

MapEdge::MapEdge(MapComponent *parent) :
    PositionalSubComponent(parent)
{
    m_pVertex0 = m_pVertex1 = NULL;
}

MapEdge::MapEdge(MapVertex *v0, MapVertex *v1, MapComponent *parent) :
    PositionalSubComponent(parent)
{
    m_pVertex0 = v0;
    m_pVertex1 = v1;
    manageChangeSignals(NULL, m_pVertex0);
    manageChangeSignals(NULL, m_pVertex1);
}

void MapEdge::setVertex0(MapVertex *vertex)
{
    if ( m_pVertex0 == vertex ) return;

    manageChangeSignals(m_pVertex0, vertex);
    m_pVertex0 = vertex;
    emit vertex0Changed(m_pVertex0);
    sendRenderDataChange();
}

void MapEdge::setVertex1(MapVertex *vertex)
{
    if ( m_pVertex1 == vertex ) return;

    manageChangeSignals(m_pVertex1, vertex);
    m_pVertex1 = vertex;
    emit vertex1Changed(m_pVertex1);
    sendRenderDataChange();
}

void MapEdge::setVertex(int index, MapVertex *vertex)
{
    if ( index == 0 ) setVertex0(vertex);
    else if ( index == 1 ) setVertex1(vertex);
}

MapVertex* MapEdge::vertex0()
{
    return m_pVertex0;
}

const MapVertex* MapEdge::vertex0() const
{
    return m_pVertex0;
}

MapVertex* MapEdge::vertex1()
{
    return m_pVertex1;
}

const MapVertex* MapEdge::vertex1() const
{
    return m_pVertex1;
}

void MapEdge::manageChangeSignals(MapVertex *vOld, MapVertex *vNew)
{
    if ( vOld )
    {
        disconnect(vOld, SIGNAL(renderDataChanged(ChangeId_t)), this, SIGNAL(renderDataChanged(ChangeId_t)));
    }

    if ( vNew )
    {
        connect(vNew, SIGNAL(renderDataChanged(ChangeId_t)), this, SIGNAL(renderDataChanged(ChangeId_t)));
    }
}

bool MapEdge::isValid() const
{
    return m_pVertex0 && m_pVertex1;
}

QBox3D MapEdge::boundingBox() const
{
    if ( !isValid() ) return QBox3D();

    return QBox3D(m_pVertex0->position(), m_pVertex1->position());
}

QVector3D MapEdge::centroid() const
{
    if ( !isValid() ) return QVector3D();

    return (vertex0()->position() + m_pVertex1->position()) / 2.0f;
}

MODEL_END_NAMESPACE
