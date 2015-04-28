#include "mapvertex.h"
#include "mapdocument.h"

MODEL_BEGIN_NAMESPACE

MapVertex::MapVertex(QObject *parent) :
    MapComponent(parent), m_vecPosition(QVector3D()), m_colColour(QColor(255,255,255)),
    m_pDocument(NULL)
{
}

MapVertex::MapVertex(const QVector3D &pos, const QColor &col, QObject *parent) :
    MapComponent(parent), m_vecPosition(pos), m_colColour(col),
    m_pDocument(NULL)
{
}

MapVertex::MapVertex(const QVector3D &pos, QObject *parent) :
    MapComponent(parent), m_vecPosition(pos), m_colColour(QColor(255,255,255)),
    m_pDocument(NULL)
{
}

QVector3D MapVertex::position() const
{
    return m_vecPosition;
}

void MapVertex::setPosition(const QVector3D &position)
{
    m_vecPosition = position;
    emit positionChanged(m_vecPosition);
    emit vertexRenderDataChanged();
}

QColor MapVertex::color() const
{
    return m_colColour;
}

void MapVertex::setColor(const QColor &colour)
{
    m_colColour = colour;
    emit colorChanged(m_colColour);
    emit vertexRenderDataChanged();
}

MapDocument* MapVertex::document()
{
    return m_pDocument;
}

const MapDocument* MapVertex::document() const
{
    return m_pDocument;
}

void MapVertex::setDocument(MapDocument *doc)
{
    m_pDocument = doc;
}

MODEL_END_NAMESPACE
