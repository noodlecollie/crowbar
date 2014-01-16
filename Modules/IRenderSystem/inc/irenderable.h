#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include "irendersystem_global.h"

#include <Qt3D/QGeometryData>

IRENDERSYSTEM_BEGIN_NAMESPACE

class IRenderable
{
public:
    virtual ~IRenderable() {}
    
    // Exports this renderable object's data to a QGeometryData object.
    virtual QGeometryData toGeomData() const = 0;
};

IRENDERSYSTEM_END_NAMESPACE

#endif // IRENDERABLE_H
