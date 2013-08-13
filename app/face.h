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

    // Get
    /**
     * @brief Gets this face's ID.
     * @return ID of face, or 0 if unassigned.
     */
    inline GEOMHANDLE getId() const                                 { return m_hID; }

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

    // Set
    /**
     * @brief Sets the face's ID.
     * @param id ID to set.
     */
    inline void setId(const GEOMHANDLE id)                          { m_hID = id; }

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
     * @brief Adds an edge to the face's edge list.
     * @note When checking for existence, this does not guarantee that only one instance of the specified edge
     * is present as the function only looks for the first occurrence of an edge. Other subsequent edge clones
     * could have been added without an existence check.
     * @param edge Edge to add.
     * @param checkForExistence If true, the edge will not be added if it is already found in the edge list.
     * @return True if the edge was added, false if an existence check was run and the edge was already found.
     */
    inline bool addEdge(const GEOMHANDLE edge, const bool checkForExistence = false);

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

    GEOMHANDLE          m_hID;
    QList<GEOMHANDLE>*  m_hEdges;
    Plane               m_Plane;
    QVector3D           m_Centre;
};

#endif // FACE_H
