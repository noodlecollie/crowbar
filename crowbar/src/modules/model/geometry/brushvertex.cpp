#include "brushvertex.h"

MODEL_BEGIN_NAMESPACE

BrushVertex::BrushVertex(QObject *parent) : QObject(parent)
{

}

QVector3D BrushVertex::position() const
{
    return m_vecPosition;
}

void BrushVertex::setPosition(const QVector3D &pos)
{
    if ( m_vecPosition == pos ) return;
    m_vecPosition = pos;
    emit positionChanged(m_vecPosition);
}

MODEL_END_NAMESPACE
