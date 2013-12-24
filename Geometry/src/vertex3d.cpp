#include <QVector3D>
#include <QVector2D>
#include <QColor>

#include "vertex3d.h"
#include "geomutil.h"

namespace Geometry
{
    Vertex3D::Vertex3D(QObject *parent) : GeomMetaHandle(parent),
        m_vecPosition(VEC3_ORIGIN), m_colColour(QColor(255,255,255,255)), m_vecTexUV(VEC2_ORIGIN)
    {
    }
    
    Vertex3D::Vertex3D(QVector3D &position, QObject *parent) : GeomMetaHandle(parent), m_vecPosition(position),
        m_colColour(QColor(255,255,255,255)), m_vecTexUV(VEC2_ORIGIN)
    {
    }
    
    void Vertex3D::setPosition(QVector3D &pos)
    {
        m_vecPosition = pos;
    }
    
    QVector3D Vertex3D::position() const
    {
        return m_vecPosition;
    }
    
    void Vertex3D::setColor(QColor &col)
    {
        m_colColour = col;
    }
    
    QColor Vertex3D::color() const
    {
        return m_colColour;
    }
    
    void Vertex3D::setTexUV(QVector2D &uv)
    {
        m_vecTexUV = uv;
    }
    
    QVector2D Vertex3D::texUV() const
    {
        return m_vecTexUV;
    }
    
    virtual bool Vertex3D::isValid() const
    {
        // Right now we don't have any specific criteria for being valid.
        return true;
    }
}
