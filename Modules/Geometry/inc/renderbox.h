#ifndef RENDERBOX_H
#define RENDERBOX_H

#include "geometry_global.h"

#include <Qt3D/QBox3D>
#include <QVector3D>
#include <QColor>

#include "igeomdataexport.h"

GEOMETRY_BEGIN_NAMESPACE

class GEOMETRYSHARED_EXPORT RenderBox : public QBox3D, public IGeomDataExport
{
public:
    explicit RenderBox();
    explicit RenderBox(const QVector3D &origin, const QVector3D &min, const QVector3D &max);
    
    // === All functions inherited from QBox3D refer to the local space of this object. ===

    QColor color() const;
    void setColor(const QColor &col);
    
    // Physical origin of this box in the world.
    QVector3D wOrigin() const;
    void setWOrigin(const QVector3D &pos);
    
    // Centre of the rendered box (which may be different from the origin),
    // in world space. center() returns in local space.
    QVector3D wCenter() const;
    
    // IGeomDataExport interface
    virtual QGeometryData toGeomData() const;
    
private:
    QVector3D   m_vecPosition;
    QColor      m_colColour;
};
    
GEOMETRY_END_NAMESPACE

#endif // RENDERBOX_H
