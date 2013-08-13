/*! \file vertex.h
 * \brief Defines 3D vertex class used by geometry.
 *
 * A vertex has a position and an ID that is unique to the piece of geometry is is part of. The ID is unsigned.
 * If the vertex has no ID, getId() will return 0 (NULLHND).
 */

#ifndef VERTEX_H
#define VERTEX_H

#include "matlib.h"
#include <QVector3D>
#include <QColor>
#include "ivertex3drenderspec.h"

typedef unsigned long GEOMHANDLE;
#define NULLHND 0

class GeomMetaHandle
{
public:
    GeomMetaHandle() : m_bSelected(false)
    {
    }

    bool m_bSelected;
};

/**
 * @brief Defines a vertex in 3D space. The vertex has a position and an integer ID that should be
 * unique amongst the elements of the pieceof geometry it belongs to.
 */
class Vertex3D : public GeomMetaHandle, public IVertex3DRenderSpec
{
public:
    /**
     * @brief Constructor. Initialises position and ID to zero.
     */
    Vertex3D() : m_Position(VEC3_ORIGIN), m_hID(NULLHND), m_Colour(255, 255, 255)
    {
    }

    /**
     * @brief Constructor specifying vertex position.
     * @param x X position.
     * @param y Y position.
     * @param z Z position.
     */
    Vertex3D(const float x, const float y, const float z) : m_Position(QVector3D(x, y, z)), m_hID(NULLHND), m_Colour(255, 255, 255)
    {
    }

    /**
     * @brief Constructor specifying vertex position.
     * @param vec Vector representing position.
     */
    Vertex3D(const QVector3D vec) : m_Position(vec), m_hID(NULLHND), m_Colour(255, 255, 255)
    {
    }

    // Get
    /**
     * @brief Gets the vertex's ID.
     * @return ID (0 if unassigned).
     */
    inline GEOMHANDLE getId() const { return m_hID; }

    /**
     * @brief Gets the vertex's position local to its parent geometry object.
     * @return Vector representing position.
     */
    inline QVector3D getPosition() const { return m_Position; }

    /**
     * @brief Gets the vertex's render colour.
     * @return Render colour.
     */
    inline QColor getColour() const { return m_Colour; }

    // Set
    /**
     * @brief Sets the vertex's ID.
     * @param id ID to set.
     */
    inline void setId(const GEOMHANDLE id) { m_hID = id; }

    /**
     * @brief Sets the vertex's position.
     * @param x X position.
     * @param y Y position.
     * @param z Z position.
     */
    inline void setPosition(const float x, const float y, const float z) { m_Position = QVector3D(x, y, z); }

    /**
     * @brief Sets the vertex's position.
     * @param pos Vector representing position.
     */
    inline void setPosition(const QVector3D pos) { m_Position = pos; }

    /**
     * @brief Sets the vertex's render colour.
     * @param colour Colour to set.
     */
    inline void setColour(const QColor colour) { m_Colour = colour; }

    // IVertex3DRenderSpec
    /**
     * @brief Fills an array with the position values for this vertex.
     * @param position Array to fill. Format is XYZ.
     */
    virtual void V3RS_Position(float position[]) { position[0] = m_Position.x(); position[1] = m_Position.y(); position[2] = m_Position.z(); }

    /**
     * @brief Fills an array with the colour values for this vertex.
     * @param colour Colour values. Format is RGBA, range 0.0 - 1.0.
     */
    virtual void V3RS_Colour(unsigned char colour[])
    {
        colour[0] = (unsigned char)m_Colour.red();
        colour[1] = (unsigned char)m_Colour.green();
        colour[2] = (unsigned char)m_Colour.blue();
        colour[3] = (unsigned char)m_Colour.alpha();
    }

    /**
     * @brief Fils an array with the normal vector values for this vertex.
     * @param normal Normal array to fill. Format is XYZ.
     */
    virtual void V3RS_Normal(float normal[]) { normal[0] = m_Normal.x(); normal[1] = m_Normal.y(); normal[2] = m_Normal.z(); }

private:
    QVector3D   m_Position; /**< Vector representing this vertex's position. */
    QVector3D   m_Normal;   /**< Vector representing this vertex's normal. */
    GEOMHANDLE  m_hID;      /**< Vertex's ID - 0 if unassigned. */
    QColor      m_Colour;   /**< Vertex's render colour. */
};

#endif // VERTEX_H
