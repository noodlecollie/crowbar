#include "mapvertex.h"

MODEL_BEGIN_NAMESPACE

MapVertex::MapVertex(MapComponent *parent) :
    PositionalSubComponent(parent)
{
    m_vecPosition = QVector3D(0,0,0);
    m_colColour = QColor(255,255,255);
}

MapVertex::MapVertex(const QVector3D &position, const QColor &colour, MapComponent *parent) :
    PositionalSubComponent(parent)
{
    m_vecPosition = position;
    m_colColour = colour;
}

MapVertex::MapVertex(const QVector3D &position, MapComponent *parent) :
    PositionalSubComponent(parent)
{
    m_vecPosition = position;
    m_colColour = QColor(255,255,255);
}

QVector3D MapVertex::position() const
{
    return m_vecPosition;
}

void MapVertex::setPosition(const QVector3D &pos)
{
    if ( pos == m_vecPosition ) return;

    m_vecPosition = pos;
    emit positionChanged(m_vecPosition);
    sendRenderDataChange();
}

QColor MapVertex::color() const
{
    return m_colColour;
}

void MapVertex::setColor(const QColor &col)
{
    if ( col == m_colColour ) return;

    m_colColour = col;
    emit colorChanged(m_colColour);
    sendRenderDataChange();
}

QBox3D MapVertex::boundingBox() const
{
    return QBox3D(position(), position());
}

QVector3D MapVertex::centroid() const
{
    return position();
}

MODEL_END_NAMESPACE
