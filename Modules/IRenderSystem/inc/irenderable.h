/** \file irenderable.h
 * \brief Defines the interface that renderables must implement to be rendered in the 3D views.
 */

#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include "irendersystem_global.h"

#include <Qt3D/QGeometryData>

IRENDERSYSTEM_BEGIN_NAMESPACE

/**
 * @brief Renderables must implement this interface in order for their data to be passed to the renderer.
 */

class IRenderable
{
public:
    // Exports this renderable object's data to a QGeometryData object.
    /**
     * @brief Exports the geometry data from this renderable. Data should be in world co-ordinates.
     * @return QGeometryData containing the vertex/normal data/texture co-ordinates for the renderable.
     */
    virtual QGeometryData toGeomData() const = 0;
    
    // TODO: Continue - we need things like texture name, etc.

};

IRENDERSYSTEM_END_NAMESPACE

#endif // IRENDERABLE_H
