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
    explicit RenderBox(const QVector3D &corner1, const QVector3D &corner2);
    
    QColor color() const;
    void setColor(const QColor &col);
    
    // IGeomDataExport interface
    virtual QGeometryData toGeomData() const;
    
private:
    QColor  m_colColour;
};
    
GEOMETRY_END_NAMESPACE

#endif // RENDERBOX_H
