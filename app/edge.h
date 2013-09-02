/*! \file edge.h
 * \brief Defines an edge which links two vertices.
 */

#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

class Solid3D;

/**
 * @brief An edge links two vertices and two faces, referenced by their geometry handles.<br/>
 * When considering travelling along the edge from the beginning to end vertex,
 * the normals of the faces either side of the edge can be considered to point clockwise or anticlockwise around the edge. The face
 * with an anticlockwise-pointing normal is denoted the "right" face (if the normal were pointing upwards the face would be positioned
 * to the right of the edge) and the face with the clockwise-pointing normal is denoted the "left" face. If both normals are pointing
 * in the same direction (either clockwise or anticlockwise) it is not defined which face is denoted left and which right.
 */
class Edge3D : public GeomMetaHandle
{
public:
    /**
     * @brief Constructor. Member variables are set to zero values.
     */
    Edge3D() : m_hVStart(NULLHND), m_hVEnd(NULLHND), m_hFRight(NULLHND), m_hFLeft(NULLHND), m_Centre(VEC3_ORIGIN),
        m_hParentSolid(NULLHND), m_hHandle(NULLHND)
    {
    }

    /**
     * @brief Constructor specifying start and end vertices and the midpoint of the edge.
     * @param start ID of vertex this edge starts at.
     * @param end ID of vertex this edge ends at.
     * @param midpoint Co-ordinates of the edge's midpoint.
     */
    Edge3D(const GEOMHANDLE start, const GEOMHANDLE end, const QVector3D midpoint)
        : m_hVStart(start), m_hVEnd(end), m_hFRight(NULLHND), m_hFLeft(NULLHND), m_Centre(midpoint),
          m_hParentSolid(NULLHND), m_hHandle(NULLHND)
    {
    }

    /**
     * @brief Constructor specifying faces as well as vertices.
     * @param start ID of vertex this edge starts at.
     * @param end ID of vertex this edge ends at.
     * @param rightface Face to the right of this edge (see page description).
     * @param leftface Face to the left of this edge (see page description).
     * @param midpoint Co-ordinates of the edge's midpoint.
     */
    Edge3D(const GEOMHANDLE start, const GEOMHANDLE end, const GEOMHANDLE rightface, const GEOMHANDLE leftface, const QVector3D midpoint)
        : m_hVStart(start), m_hVEnd(end), m_hFRight(rightface), m_hFLeft(leftface), m_Centre(midpoint),
          m_hParentSolid(NULLHND), m_hHandle(NULLHND)
    {
    }

    // ===== Begin get functions =====
    
    /**
     * @brief Returns the start vertex for this edge.
     * @return Start vertex ID.
     */
    inline GEOMHANDLE getStartVertex() const                { return m_hVStart; }

    /**
     * @brief Returns the end vertex of this edge.
     * @return End vertex ID.
     */
    inline GEOMHANDLE getEndVertex() const                  { return m_hVEnd; }

    /**
     * @brief Returns the edge's right face.
     * @return Right face ID (see page description).
     */
    inline GEOMHANDLE getRightFace() const                  { return m_hFRight; }

    /**
     * @brief Returns the edge's left face.
     * @return Left face ID (see page description).
     */
    inline GEOMHANDLE getLeftFace() const                   { return m_hFLeft; }

    /**
     * @brief Returns the edge's midpoint.
     * @return Co-ordinates of midpoint of edge.
     */
    inline QVector3D getMidpoint() const                    { return m_Centre; }
    
	/**
	 * @brief Returns the handle of the parent solid of this edge.
	 * @return Parent solid handle.
	 */
    inline GEOMHANDLE getParentSolid() const                { return m_hParentSolid; }

	/**
	 * @brief Returns this edge's geometry handle.
	 * @return Handle for this edge.
	 */
    inline GEOMHANDLE getHandle() const                     { return m_hHandle; }
    
    // ===== End get functions =====
    
    // ===== Begin set functions =====
    /**
     * @brief Sets the edge's start vertex.
     * @param v Vertex ID to set.
     */
    inline void setStartVertex(const GEOMHANDLE v)          { m_hVStart = v; }

    /**
     * @brief Sets the edge's end vertex.
     * @param v Vertex ID to set.
     */
    inline void setEndVertex(const GEOMHANDLE v)            { m_hVEnd = v; }

    /**
     * @brief Sets the edge's right face.
     * @param f Face ID to set.
     */
    inline void setRightFace(const GEOMHANDLE f)            { m_hFRight = f; }

    /**
     * @brief Sets the edge's left face.
     * @param f Face ID to set.
     */
    inline void setLeftFace(const GEOMHANDLE f)             { m_hFLeft = f; }

    /**
     * @brief Sets the edge's midpoint.
     * @param mid Co-ordinates of midpoint.
     */
    inline void setMidpoint(const QVector3D mid)            { m_Centre = mid; }
    
	/**
	 * @brief Sets the parent solid handle for this edge.
	 * @param handle Handle to set.
	 */
    inline void setParentSolid(const GEOMHANDLE handle)     { m_hParentSolid = handle; }
    
	/**
	 * @brief Sets this edge's handle.
	 * @param handle Handle to set.
	 */
    inline void setHandle(const GEOMHANDLE handle)          { m_hHandle = handle; }
    
    // ===== End set functions =====
    
    // Calculates mid-point between the two vertices when a parent solid is provided.
	/**
	 * @brief Given a parent solid, calculates the mid-point of the edge by looking up the vertices it references.
	 * @param parent Parent solid of this edge.
	 */
    bool calcMidpoint(Solid3D& parent);
    
    // TODO: Change this to find the parent from the global brush list.

private:
    // Handles
    GEOMHANDLE m_hParentSolid;  /**< Global handle of parent solid this edge belongs to. */
    GEOMHANDLE m_hHandle;
    
    GEOMHANDLE m_hVStart;       /**< Start vertex of edge. */
    GEOMHANDLE m_hVEnd;         /**< End vertex of edge. */
    GEOMHANDLE m_hFRight;       /**< Right face of edge. */
    GEOMHANDLE m_hFLeft;        /**< Left face of edge. */
    QVector3D  m_Centre;        /**< Midpoint of edge. */
};

#endif // EDGE_H
