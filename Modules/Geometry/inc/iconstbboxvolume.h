#ifndef ICONSTBBOXVOLUME_H
#define ICONSTBBOXVOLUME_H

#include "geometry_global.h"
#include <Qt3D/QBox3D>

GEOMETRY_BEGIN_NAMESPACE

/**
 * @brief Interface for an object with a 3D bounding box presence.
 * 
 * This interface does not allow for setting the bounding box, only for retrieving it.
 */
class GEOMETRYSHARED_EXPORT IConstBBoxVolume
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IConstBBoxVolume() {}
    
    /**
     * @brief Dummy function. To check at compile-time whether a type implements
     * this interface, call this function. A compiler error will occur if the
     * interface is not present.
     * @return Returns true.
     */
    virtual bool _implementsIConstBBoxVolume() const = 0;
    
    /**
     * @brief Gets the bounding box for this object.
     * @return Bounding box.
     */
    virtual QBox3D boundingBox() const = 0;
};

GEOMETRY_END_NAMESPACE

#endif // ICONSTBBOXVOLUME_H
