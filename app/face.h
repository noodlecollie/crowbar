/*! \file face.h
 * \brief Defines a 3D face. Solids are built up of these faces. Faces reference edges, which in turn reference vertices.
 */

#ifndef FACE_H
#define FACE_H

#include "edge.h"
#include <QList>
#include "plane.h"

/**
 * @brief Class representing a 3D face.
 * 
 * Solids are made up of faces, which are built up from lists of member edges. Faces also contain plane information (for
 * easy conversion to/from VMFs). The direction of the face's edges according to their start/end vertices does not necessarily
 * have to be clockwise (this would be impractical) but the edges do have to form a closed, contiguous loop from the beginning
 * to the end of the list, the last vertex of the last edge being the first vertex of the first edge. The loop should form a
 * convex 2D polygon.
 */
class Face3D : public GeomMetaHandle
{
public:
    /**
     * @brief Constructor. Sets member variables to default values.
     */
    Face3D() : m_hEdges(NULL), m_Plane(), m_Centre(VEC3_ORIGIN), m_hParentSolid(NULLHND), m_hHandle(NULLHND)
    {
        init();
    }

    /**
     * @brief Constructor specifying plane and centre point of face.
     * @param p
     * @param centre
     */
    Face3D(const Plane p, const QVector3D centre = VEC3_ORIGIN) : m_Plane(p), m_Centre(centre), m_hParentSolid(NULLHND), m_hHandle(NULLHND)
    {
        init();
    }

    /**
     * @brief Destructor.
     */
    ~Face3D()
    {
        if ( m_hEdges )
        {
            delete m_hEdges;
            m_hEdges = NULL;
        }
    }

    // ===== Begin get functions =====
    
    /**
     * @brief Returns the number of edges this face contains.
     * @return Number of edges.
     */
    inline int getEdgeCount() const                                 { return m_hEdges->size(); }

    /**
     * @brief Returns the handle to the edge at a specific index in the edge list. Index must be within range.
     * @param index Index of edge to retrieve.
     * @return Edge at this index.
     */
    inline GEOMHANDLE getEdgeHandle(const int index) const          { Q_ASSERT(index < m_hEdges->size() && index >= 0); return m_hEdges->at(index); }

    /**
     * @brief Gets the plane that represents this face's surface.
     * @return Plane of face.
     */
    inline Plane getPlane() const                                   { return m_Plane; }

    /**
     * @brief Gets this face's normal.
     * @return Normal of face.
     */
    inline QVector3D getNormal() const                              { return m_Plane.getNormal(); }

    /**
     * @brief Returns whether the face's plane is valid.
     * @return True if plane is valid, false otherwise.
     */
    inline bool isPlaneValid() const                                { return m_Plane.isValid(); }

    /**
     * @brief Gets the central point of the face.
     * @return Face centre point.
     */
    inline QVector3D getCentrePoint() const                         { return m_Centre; }
    
	/**
	 * @brief Gets the hand of the parent solid of this face.
	 * @return Parent solid handle.
	 */
    inline GEOMHANDLE getParentSolid() const                        { return m_hParentSolid; }
    
	/**
	 * @brief Gets this face's handle.
	 * @return Handle of the face.
	 */
    inline GEOMHANDLE getHandle() const                             { return m_hHandle; }
    
	/**
	 * @brief Returns whether this face contains an edge with the specified handle.
	 * @param edge Handle to check.
	 * @return True if the edge is contained within the face, false if not.
	 */
    bool containsEdge(const GEOMHANDLE edge) const;
    
	/**
	 * @brief Returns whether this face contains any of the edges in the list provided.
	 * @param edges List of edge handles to check.
	 * @return True if the face contains any edge, false if the face contains none of the edges.
	 */
    bool containsAnyEdge(const QList<GEOMHANDLE>& edges) const;
    
    // ===== End get functions =====
    
    // ===== Begin set functions =====

    /**
     * @brief Set's the face's plane.
     * @param p Plane to set.
     */
    inline void setPlane(const Plane p)                             { m_Plane = p; }

    /**
     * @brief Sets the face's central point.
     * @param centre Centre point to set.
     */
    inline void setCentrePoint(const QVector3D centre)              { m_Centre = centre; }
    
	/**
	 * @brief Sets the parent solid handle for this face.
	 * @param handle Handle to set.
	 */
    inline void setParentSolid(const GEOMHANDLE handle)             { m_hParentSolid = handle; }
    
	/**
	 * @brief Sets the handle of this face.
	 * @param handle Handle to set.
	 */
    inline void setHandle(const GEOMHANDLE handle)                  { m_hHandle = handle; }
    
	/**
	 * @brief Adds an edge to this face. The edge must already exist in the face's parent solid.
	 * @note Does NOT check whether the edge already exists in the face.
	 * @param edge Handle of the edge to add.
	 */
    inline void addEdge(GEOMHANDLE edge);
    
    // ===== End set functions =====

    /**
     * @brief Returns whether the face is composite. Composite faces contain more than
     * just three edges (ie. they are composed of more than one triangle).
     * @return True if face is composite, false otherwise.
     */
    inline bool isComposite()                                       { return m_hEdges->size() > 3; }

    // TODO: Add function testing whether face is valid - no vertex must be used more than once
    // per start/end in the face list apart from the beginning vertex, which must be v1 in the
    // starting edge and v2 in the ending edge.

private:
	/**
	 * @brief Performs initialisation when the face is created.
	 */
    inline void init()
    {
        if ( m_hEdges )
        {
            delete m_hEdges;
            m_hEdges = NULL;
        }

        m_hEdges = new QList<GEOMHANDLE>();
    }

	/**
	 * @brief Compresses the memory required by the edge list, if possible.
	 *
	 * When edges are deleted from the edge list, the list shrinks. The memory allocated for the list object, however, does not.
	 * This function will delete and recreate the edge list so that it uses only the currently required amount of memory.
	 * It is unlikely that this function will need to be used much, however, as: A. Faces shouldn't really ever contain a large
	 * number of edges, and B. edges are not removed from faces that often.
	 */
    void compressEdgeMemory()
    {
        if ( !m_hEdges ) return;

        // Create a new list from the member list.
        // This will remove any superfluous memory usage where old
        // elements may have been deleted from when the list was larger.
        QList<GEOMHANDLE>* newList = new QList<GEOMHANDLE>(*m_hEdges);
        delete m_hEdges;
        m_hEdges = newList;
    }

    // TODO: Add function to calculate centre point given vertices from a solid.

    // Handles
    GEOMHANDLE          m_hParentSolid;     /**< Global handle of parent solid this face belongs to. */
    GEOMHANDLE          m_hHandle;			/**< Handle of this face within the parent solid. */
    
    QList<GEOMHANDLE>*  m_hEdges;			/**< List of handles of edges that this face contains. */
    Plane               m_Plane;			/**< Plane representing the plane of this face. */
    QVector3D           m_Centre;			/**< The mid-point of this face. */
};

#endif // FACE_H
