#ifndef IGEOMDATAEXPORT_H
#define IGEOMDATAEXPORT_H

#include "geometry_global.h"
#include <Qt3D/QGeometryData>

GEOMETRY_BEGIN_NAMESPACE

class GEOMETRYSHARED_EXPORT IGeomDataExport
{
public:
    virtual ~IGeomDataExport() {}
    
    // Export to QGeometryData, in world co-ordinates.
    virtual QGeometryData toGeomData() const = 0;
};

GEOMETRY_END_NAMESPACE

#endif // IGEOMDATAEXPORT_H
