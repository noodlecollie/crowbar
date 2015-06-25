#include "brushedge.h"
#include "brushvertex.h"

MODEL_BEGIN_NAMESPACE

BrushEdge::BrushEdge(QObject *parent) : QObject(parent)
{

}

BrushEdge::BrushEdge(BrushVertex *v0, BrushVertex *v1, QObject *parent) : QObject(parent)
{
    m_pVertex0 = v0;
    m_pVertex1 = v1;
}

bool BrushEdge::operator ==(const BrushEdge &other) const
{
    return (vertex0() == other.vertex0() && vertex1() == other.vertex1()) ||
           (vertex0() == other.vertex1() && vertex1() == other.vertex0());
}

BrushVertex* BrushEdge::vertex0() const
{
    return m_pVertex0.data();
}

void BrushEdge::setVertex0(BrushVertex *v)
{
    if ( v == m_pVertex0.data() ) return;

    m_pVertex0 = v;
    emit vertex0Changed(m_pVertex0.data());
}

BrushVertex* BrushEdge::vertex1() const
{
    return m_pVertex1.data();
}

void BrushEdge::setVertex1(BrushVertex *v)
{
    if ( v == m_pVertex1.data() ) return;

    m_pVertex1 = v;
    emit vertex1Changed(m_pVertex1.data());
}

bool BrushEdge::isValid() const
{
    return !m_pVertex0.isNull() && !m_pVertex1.isNull();
}

MODEL_END_NAMESPACE
