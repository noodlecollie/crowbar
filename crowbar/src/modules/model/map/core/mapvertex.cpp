#include "mapvertex.h"

MODEL_BEGIN_NAMESPACE

MapVertex::MapVertex(QObject *parent) : MapHandleObject(parent)
{
    
}

MapVertex::~MapVertex()
{

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
    generateChange(ChangeNotifier::FlagRender);
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
    generateChange(ChangeNotifier::FlagRender);
}

QVector2D MapVertex::textureCoordinate() const
{
    return m_vecTexCoord;
}

void MapVertex::setTextureCoordinate(const QVector2D &coord)
{
    if ( coord == m_vecTexCoord ) return;

    m_vecTexCoord = coord;
    emit textureCoordinateChanged(m_vecTexCoord);
    generateChange(ChangeNotifier::FlagRender);
}

MODEL_END_NAMESPACE
