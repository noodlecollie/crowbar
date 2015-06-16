#include "mapedge.h"

MODEL_BEGIN_NAMESPACE

MapEdge::MapEdge(QObject *parent) : MapHandleObject(parent)
{

}

MapEdge::~MapEdge()
{

}

MapVertex* MapEdge::vertex0() const
{
    return m_pVertex0.data();
}

void MapEdge::setVertex0(MapVertex *v0)
{
    if ( v0 == m_pVertex0.data() ) return;

    m_pVertex0 = v0;
    emit vertex0Changed(m_pVertex0.data());
    generateChange(ChangeNotifier::FlagRender);
}

MapVertex* MapEdge::vertex1() const
{
    return m_pVertex1.data();
}

void MapEdge::setVertex1(MapVertex *v1)
{
    if ( v1 == m_pVertex1.data() ) return;

    m_pVertex1 = v1;
    emit vertex1Changed(m_pVertex1.data());
    generateChange(ChangeNotifier::FlagRender);
}

MODEL_END_NAMESPACE
