/*! \file vertex.h
 * \brief TODO
 */

#ifndef VERTEX_H
#define VERTEX_H

#include "matlib.h"
#include <QVector3D>
#include <QColor>
#include "ivertex3drenderspec.h"

typedef unsigned long GEOMHANDLE;
typedef unsigned long VBO_OFFSET;
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
 * @brief Defines a vertex in 3D space. The vertex has an integer ID that should be
 * unique amongst the elements of the pieceof geometry it belongs to. This ID does
 * not have to be consecutive with the previous ID; the VBO offset, however, should
 * be. TODO: Document this all better.
 */
class Vertex3D : public GeomMetaHandle, public IVertex3DRenderSpec
{
public:
    /**
     * @brief Constructor. Initialises position and ID to zero.
     */
    Vertex3D() : m_Position(VEC3_ORIGIN), m_Normal(VEC3_ORIGIN), m_hParentSolid(NULLHND), m_hVBOOffset(NULLHND),
        m_Colour(255, 255, 255), m_flTexX(0.0), m_flTexY(0.0)
    {
    }

    /**
     * @brief Constructor specifying vertex position.
     * @param x X position.
     * @param y Y position.
     * @param z Z position.
     */
    Vertex3D(const float x, const float y, const float z) : m_Position(QVector3D(x, y, z)), m_hParentSolid(NULLHND),
        m_hVBOOffset(NULLHND), m_Colour(255, 255, 255), m_flTexX(0.0), m_flTexY(0.0), m_Normal(VEC3_ORIGIN)
    {
    }

    /**
     * @brief Constructor specifying vertex position.
     * @param vec Vector representing position.
     */
    Vertex3D(const QVector3D vec) : m_Position(vec), m_hParentSolid(NULLHND), m_Colour(255, 255, 255),
        m_flTexX(0.0), m_flTexY(0.0), m_Normal(VEC3_ORIGIN), m_hVBOOffset(NULLHND)
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
    
    inline VBO_OFFSET getVBOOffset() const { return m_hVBOOffset; }
    
    inline GEOMHANDLE getParentSolid() const { return m_hParentSolid; }
    
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
    
    inline void setVBOHandle(const VBO_OFFSET handle) { m_hVBOOffset = handle; }
    
    inline void setParentSolid(const GEOMHANDLE handle) { m_hParentSolid = handle; }
    
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
     * @brief Fils an array with the normal vector values for this vertex.
     * @param normal Normal array to fill. Format is XYZ.
     */
    virtual void V3RS_Normal(float normal[]) { normal[0] = m_Normal.x(); normal[1] = m_Normal.y(); normal[2] = m_Normal.z(); }

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
