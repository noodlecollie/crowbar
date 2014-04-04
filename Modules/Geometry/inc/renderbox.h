/** \file renderbox.h
 * \brief Defines the RenderBox class, which is used to specify an axially-aligned box in 3D space.
 */

#ifndef RENDERBOX_H
#define RENDERBOX_H

#include "geometry_global.h"

#include <Qt3D/QBox3D>
#include <QVector3D>
#include <QColor>

#include "irenderable.h"
#include "iconstbboxvolume.h"

GEOMETRY_BEGIN_NAMESPACE

/**
 * @brief Defines an axially-aligned box in 3D space.
 * 
 * The RenderBox's max and min points are specified by vectors and are relative to its origin (ie. the origin position
 * need not be in the centre of the box volume). The box's min and max points can also be coloured, and the colour
 * will be interpolated between the points if different.
 */
class GEOMETRYSHARED_EXPORT RenderBox : public QBox3D, public IRENDERSYSTEM_NAMESPACE::IRenderable, public IConstBBoxVolume
{
public:
   /**
    * @brief Default constructor.
    */
    explicit RenderBox();
    /**
     * @brief Constructor.
     * @param origin Origin of the box, in world co-ordinates.
     * @param min Min point of the box, local to its origin.
     * @param max Max point of the box, local to its origin.
     */
    explicit RenderBox(const QVector3D &origin, const QVector3D &min, const QVector3D &max);
    
    // === All functions inherited from QBox3D refer to the local space of this object. ===
    
    /**
     * @brief Returns the colour at the minimum point of the box.
     * @return Colour at box's min point.
     */
    QColor colorMin() const;
    
    /**
     * @brief Sets the colour at the miminum point of the box.
     * @param col Colour to set.
     */
    void setColorMin(const QColor &col);
    
    /**
     * @brief Returns the colour at the maximum point of the box.
     * @return Colour at box's max point.
     */
    QColor colorMax() const;
    
    /**
     * @brief Sets the colour at the maximum point of the box.
     * @param col Colour to set.
     */
    void setColorMax(const QColor &col);
    
    /**
     * @brief Convenience function to set the colour of the entire box.
     * @param col Colour to set.
     */
    void setColor(const QColor &col);
    
    // Physical origin of this box in the world.
    /**
     * @brief Gets the world origin of the box.
     * @return Origin in world co-ordinates.
     */
    QVector3D wOrigin() const;
    
    /**
     * @brief Sets the world origin of the box.
     * @param pos Origin to set.
     */
    void setWOrigin(const QVector3D &pos);
    
    // Centre of the rendered box (which may be different from the origin),
    // in world space. center() returns in local space.
    /**
     * @brief Returns the centre of the box (which may be different from the origin), in world co-ordinates.
     * @note This function is different from center(), which returns in local co-ordinates.
     * @return Centre of the box in world co-ordinates.
     */
    QVector3D wCenter() const;
    
    // IRenderable interface
    /**
     * @brief Exports the geometry data from this renderable.
     * @return QGeometryData containing the vertex/normal data/texture co-ordinates for the renderable.
     */
    virtual QGeometryData toGeomData() const;
    
    // IConstBBoxVolume interface.
    /**
     * @brief Returns the bounding box of this object.
     * @return Bounding box in world space.
     */
    virtual QBox3D boundingBox() const;
    
private:
    QVector3D   m_vecPosition;  /**< Origin of the box in world space. */
    QColor      m_colColourMin; /**< Min point of the box, relative to its origin. */
    QColor      m_colColourMax; /**< Max point of the box, relative to its origin. */
};
    
GEOMETRY_END_NAMESPACE

#endif // RENDERBOX_H
