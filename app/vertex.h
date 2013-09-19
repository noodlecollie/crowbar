/*! \file vertex.h
 * \brief TODO
 */

#ifndef VERTEX_H
#define VERTEX_H

#include "matlib.h"
#include <QVector3D>
#include <QColor>
#include "ivertex3drenderspec.h"

//! \typedef GEOMHANDLE
//! \brief Handle for a geometry component.
typedef unsigned long GEOMHANDLE;

//! \typedef VBO_OFFSET
//! \brief Value for specifying an offset in the graphics VBO.
typedef unsigned long VBO_OFFSET;

//! \def NULLHND
//! \brief Value of a null GEOMHANDLE.
#define NULLHND 0

/**
 * @brief Metaclass which all geometry components subclass from. Contains useful metadata relevant to geometry components.
 */
class GeomMetaHandle
{
public:
	/**
	 * @brief Default constructor. Initialises values to null.
	 */
    GeomMetaHandle() : m_bSelected(false)
    {
    }

    bool m_bSelected;	/**< Whether the component is currently selected. */
};

/**
 * @brief Defines a vertex in 3D space.
 *
 * The vertex contains a VBO offset which, when combined with the parent solid's offset,
 * defines where in the VBO this vertex's render data is found. The vertex also contains colour
 * information and texture co-ordinates. Normals are not stored per vertex as when faces are rendered,
 * the face's stored normal is used to calculate the shading required.
 */
class Vertex3D : public GeomMetaHandle, public IVertex3DRenderSpec	// TODO: Implement the "Q Implements Interface" macro
{
public:
    /**
     * @brief Constructor. Initialises position and ID to zero.
     */
    Vertex3D() : m_hVBOOffset(NULLHND), m_hParentSolid(NULLHND), m_hHandle(NULLHND), m_Position(VEC3_ORIGIN), m_Normal(VEC3_ORIGIN),
        m_Colour(255, 255, 255), m_flTexX(0.0f), m_flTexY(0.0f)
    {
    }

    /**
     * @brief Constructor specifying vertex position.
     * @param x X position.
     * @param y Y position.
     * @param z Z position.
     */
    Vertex3D(const float x, const float y, const float z) : m_hVBOOffset(NULLHND), m_hParentSolid(NULLHND), m_hHandle(NULLHND),
        m_Position(QVector3D(x, y, z)), m_Normal(VEC3_ORIGIN), m_Colour(255, 255, 255), m_flTexX(0.0f), m_flTexY(0.0f)
    {
    }

    /**
     * @brief Constructor specifying vertex position.
     * @param vec Vector representing position.
     */
    Vertex3D(const QVector3D vec) : m_hVBOOffset(NULLHND), m_hParentSolid(NULLHND), m_hHandle(NULLHND),
        m_Position(vec), m_Normal(VEC3_ORIGIN), m_Colour(255, 255, 255), m_flTexX(0.0f), m_flTexY(0.0f)
    {
    }

    // ===== Begin get functions =====

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
    
    /**
     * @brief Gets this vertex's normal.
     * @return Normal vector.
     */
    inline QVector3D getNormal() const { return m_Normal; }
    
    /**
     * @brief Gets this vertex's X texture co-ordinate.
     * @return X co-ordinate.
     */
    inline float getTexCoordX() const { return m_flTexX; }
    
    /**
     * @brief Gets this vertex's Y texture co-ordinate.
     * @return Y co-ordinate.
     */
    inline float getTexCoordY() const { return m_flTexY; }
    
	/**
	 * @brief Gets this vertex's VBO offset from the beginning of the parent solid. This offset it
	 * not valid if the parent solid is NULLHND.
	 * @return Vertex's VBO offset.
	 */
    inline VBO_OFFSET getVBOOffset() const { return m_hVBOOffset; }
    
	/**
	 * @brief Gets the handle of this vertex's parent solid.
	 * @return Parent solid's handle, or NULLHND if no parent solid exists.
	 */
    inline GEOMHANDLE getParentSolid() const { return m_hParentSolid; }
    
	/**
	 * @brief Returns this vertex's handle, which is unique within its parent solid.
	 * @return This vertex's handle.
	 */
    inline GEOMHANDLE getHandle() const { return m_hHandle; }
    
    // ===== End get functions =====

    // ===== Begin set functions =====

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
    
    /**
     * @brief Sets this vertex's normal.
     * @param normal Normal to set.
     */
    inline void setNormal(const QVector3D normal) { m_Normal = normal; }
    
    /**
     * @brief Sets this vertex's X texture co-ordinate.
     * @param coord Co-ord to set.
     */
    inline void setTexCoordX(const float coord) { m_flTexX = coord; }
    
    /**
     * @brief Sets this vertex's Y texture co-ordinate.
     * @param coord Co-ord to set.
     */
    inline void setTexCoordY(const float coord) { m_flTexY = coord; }
    
	/**
	 * @brief Sets this vertex's VBO offset.
	 * @param offset Offset to set.
	 */
    inline void setVBOOffset(const VBO_OFFSET offset) { m_hVBOOffset = offset; }
    
	/**
	 * @brief Sets this vertex's parent solid handle.
	 * @param handle Handle to set.
	 */
    inline void setParentSolid(const GEOMHANDLE handle) { m_hParentSolid = handle; }
    
	/**
	 * @brief Sets this vertex's handle.
	 * @param handle Handle to set.
	 */
    inline void setHandle(const GEOMHANDLE handle) { m_hHandle = handle; }
    
    // ===== End set functions =====

    // ===== Begin IVertex3DRenderSpec =====
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
     * @brief Fills an array with the texture co-ordinate values for this vertex.
     * @param coords Array to fill. Format is XY.
     */
    virtual void V3RS_Texture_Coords(float coords[]) { coords[0] = m_flTexX; coords[1] = m_flTexY; }

    /**
     * @brief Returns the offset of this vertex from the beginning of the VBO,
     * in V3RS_TOTAL_DATA_TRANSFER strides.
     * @return Offset for this vertex.
     */
    virtual unsigned long V3RS_Offset() { return getVBOOffset(); }
    
    // ===== End IVertex3DRenderSpec =====
    
private:
    // Handles
    VBO_OFFSET  m_hVBOOffset;       /**< Offset from the beginning of the parent solid's VBO sector to find this vertex. Not valid if parent solid is 0. */
    GEOMHANDLE  m_hParentSolid;     /**< Global handle of parent solid this vertex belongs to. */
    GEOMHANDLE  m_hHandle;
    
    QVector3D   m_Position;         /**< Vector representing this vertex's position. */
    QVector3D   m_Normal;           /**< Vector representing this vertex's normal. */
    QColor      m_Colour;           /**< Vertex's render colour. */
    float       m_flTexX;           /**< X texture co-ordinate. */
    float       m_flTexY;           /**< Y texture co-ordinate. */
};

#endif // VERTEX_H
