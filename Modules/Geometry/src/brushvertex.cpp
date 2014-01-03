#include "brushvertex.h"
#include "geomutil.h"

GEOMETRY_BEGIN_NAMESPACE

BrushVertex::BrushVertex(QObject *parent) : GeomMetaHandle(parent),
    m_vecPosition(VEC3_ORIGIN), m_colColour(255,255,255,255)
{
}

BrushVertex::BrushVertex(const BrushVertex &other) : GeomMetaHandle(other.parent()),
    m_vecPosition(other.position()), m_colColour(other.color())
{
}

BrushVertex::BrushVertex(const QVector3D &position, const QColor &col, QObject *parent) :
    GeomMetaHandle(parent), m_vecPosition(position), m_colColour(col)
{
}

QVector3D BrushVertex::position() const
{
    return m_vecPosition;
}

void BrushVertex::setPosition(const QVector3D &pos)
{
    m_vecPosition = pos;
}

QColor BrushVertex::color() const
{
    return m_colColour;
}

void BrushVertex::setColor(const QColor &col)
{
    m_colColour = col;
}

bool BrushVertex::isValid() const
{
    // We have no specific criteria currently, so return true.
    return true;
}

QBox3D BrushVertex::boundingBox() const
{
    return QBox3D(m_vecPosition, m_vecPosition);
}

QVector3D BrushVertex::center() const
{
    return m_vecPosition;
}

void BrushVertex::translate(const QVector3D delta)
{
    // Add the delta to our current position.
    m_vecPosition += delta;
}

void BrushVertex::rotate(const QVector3D origin, const QQuaternion rot)
{
    // To rotate around a point, we treat the point like the origin.
    // To do this, translate so that the point acts as the origin, rotate and translate back again.
    
//    QVector3D trans = m_vecPosition;
//    trans -= origin;
    
//    // Rotate vector.
//    QVector rotated = rot.rotatedVector(trans);
    
//    // Translate back.
//    m_vecPosition = rotated += origin;
    
    // Quicker!
    m_vecPosition = (rot.rotatedVector(m_vecPosition - origin)) + origin;
}

GEOMETRY_END_NAMESPACE
