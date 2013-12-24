#ifndef VERTEX3D_H
#define VERTEX3D_H

#include <QVector3D>
#include <QVector2D>
#include <QColor>

#include "geommetahandle.h"

namespace Geometry
{
    class Vertex3D : public GeomMetaHandle
    {
        Q_PROPERTY(QVector3D position READ position WRITE setPosition)
        Q_PROPERTY(QColor color READ color WRITE setColor)
        Q_PROPERTY(QVector2D texUV READ texUV WRITE setTexUV)
    public:
        Vertex3D(QObject *parent = 0);
        Vertex3D(QVector3D &position, QObject *parent = 0);
        
        QVector3D position() const;
        void setPosition(QVector3D &pos);
        
        QColor color() const;
        void setColor(QColor &col);
        
        QVector2D texUV() const;
        void setTexUV(QVector2D &uv);
        
        virtual bool isValid() const;
        
    private:
        QVector3D   m_vecPosition;
        QColor      m_colColour;
        QVector2D   m_vecTexUV;
    };
}

#endif // VERTEX3D_H
