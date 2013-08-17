/*! \file face.h
 * \brief Defines a face with a collection of edges and a plane specifier.
 */

#ifndef FACE_H
#define FACE_H

#include "edge.h"
#include <QList>
#include "plane.h"

/**
 * @brief Class representing a face with a collection of edges and face plane information.
 */
class Face3D : public GeomMetaHandle
{
public:
    /**
     * @brief Constructor. Sets member variables to default values.
     */
    Face3D() : m_hID(NULLHND), m_hEdges(NULL), m_Plane(), m_Centre(VEC3_ORIGIN)
    {
        init();
    }

    /**
     * @brief Constructor specifying plane and centre point of face.
     * @param p
     * @param centre
     */
    Face3D(const Plane p, const QVector3D centre = VEC3_ORIGIN) : m_hID(NULLHND), m_Plane(p), m_Centre(centre)
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

    // ===== Begin get functions ===== \\
    
    /**
     * @brief Returns the number of edges this face contains.
     * @return Number of edges.
     */
    inline int getEdgeCount() const                                 { return m_hEdges->size(); }

    /**
     * @brief Returns the handle to the edge at a specif index in the edge list. Index must be within range.
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
    
    inline GEOMHANDLE getGlobalHandle() const                       { return m_hGlobalHandle; }
    
    inline GEOMHANDLE getParentSolid() const                        { return m_hParentSolid; }
    
    // ===== End get functions ===== \\
    
    // ===== Begin set functions ===== \\

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
    
    inline void setGlobalHandle(const GEOMHANDLE handle)            { m_hGlobalHandle = handle; }
    
    inline void setParentSolid(const GEOMHANDLE handle)             { m_hParentSolid = handle; }
    
    // ===== End set functions ===== \\

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
    inline void init()
    {
        if ( m_hEdges )
        {
            delete m_hEdges;
            m_hEdges = NULL;
        }

        m_hEdges = new QList<GEOMHANDLE>();
    }

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
    GEOMHANDLE          m_hGlobalHandle;    /**< Handle representing the face's global unique ID. Must be unique but is not necessarily consecutive. */
    GEOMHANDLE          m_hParentSolid;     /**< Global handle of parent solid this face belongs to. */
    
    QList<GEOMHANDLE>*  m_hEdges;
    Plane               m_Plane;
    QVector3D           m_Centre;
};

#endif // FACE_H
