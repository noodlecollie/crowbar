/** \file solid.h
 * \brief Defines a solid class.
 *
 * Solids are the programmatical implementation of brushes and are made up of sets of vertices, edges and faces. Each edge references the vertices it links,
 * and each face is built from multiple edges. As opposed to Hammer's implementation, solids are mesh-based (ie. they are defined by vertex positions and
 * links rather than planes and intersections) to allow for easier manipulation, and are exported as plane-based objects when serialised as a VMF.<br/>
 * It is recommended to work <strong>bottom-up</strong> (ie. add any required vertices before edges, and edges before faces) rather than
 * <strong>top-down</strong> (creating all of a face's edges and vertices as the face itself is created) as the former method is less prone to errors.
 */

#ifndef SOLID_H
#define SOLID_H

#include <QVector>
#include "face.h"
#include "indexpool.h"

/**
 * @brief Struct to hold and pass information about geometry objects.
 */
struct GeomInfo
{
public:
    
    /**
     * @brief Specifies what type of geometry we are describing.
     */
    enum GeomType
    {
        Null = 0,   /**< NULL (invalid) */
        Vertex,     /**< Vertex */
        Edge,       /**< Edge */
        Face,       /**< Face */
        Solid       /**< Solid */
    };
    
    GEOMHANDLE  m_hHandle;      /**< Local handle for this object. */
    GeomType    m_iType;        /**< Type specifier. */
    GEOMHANDLE  m_hParentSolid; /**< Handle to parent solid. NULL if m_iType is GeomType::Solid. */
};

/**
 * @brief Class representing a 3D solid.
 *
 * Solids are the programmatical implementation of brushes and are made up of sets of vertices, edges and faces. Each edge references the vertices it links,
 * and each face is built from multiple edges. As opposed to Hammer's implementation, solids are mesh-based (ie. they are defined by vertex positions and
 * links rather than planes and intersections) to allow for easier manipulation, and are exported as plane-based objects when serialised as a VMF.<br/>
 * It is recommended to work <strong>bottom-up</strong> (ie. add any required vertices before edges, and edges before faces) rather than
 * <strong>top-down</strong> (creating all of a face's edges and vertices as the face itself is created) as the former method is less prone to errors.
 */
class Solid3D : public GeomMetaHandle
{
public:
    /**
     * @brief Vertex3DList typedef.
     */
    typedef QVector<Vertex3D*> Vertex3DList;
    
    /**
     * @brief Edge3DList typedef.
     */
    typedef QVector<Edge3D*> Edge3DList;
    
    /**
     * @brief Face3DList typedef.
     */
    typedef QVector<Face3D*> Face3DList;
    
    enum
    {
        MAX_VERTICES = 384, /**< Max vertices allowed in a solid. */
        MAX_EDGES = 384,    /**< Max edges allowed in a solid. */
        MAX_FACES = 384     /**< Max faces allowed in a solid. */
    };

    /**
     * @brief Default constructor. Initialises empty members.
     */
    Solid3D();
    
    /**
     * @brief Destructor.
     */
    ~Solid3D();
    
    // ===== Begin vertex functions ===== \\
    
    /**
     * @brief Adds the specified vertex.
     *
     * The vertex's handles will be updated to reflect its place in the solid.
     * @param vertex Vertex to add.
     * @return True if addition was successful, false if it was not.
     */
    bool addVertex(Vertex3D* vertex);
    
    /**
     * @brief Removes the vertex with the specified handle.
     *
     * The vertex vector will be shrunk and other vertices' VBO handles
     * updated. The removed vertex's parent handle will be set to 0.
     * Edges/faces using this vertex are NOT updated about its removal.
     * @param vertex Handle to search for.
     * @return Vertex removed, or NULL if the vertex with the specified handle was not found.
     */
    Vertex3D* removeVertexHandle(const GEOMHANDLE vertex);
    
    /**
     * @brief Returns a pointer to the vertex with the specified handle.
     * @param vertex Handle of vertex to find.
     * @return Pointer to vertex, or NULL if not found.
     */
    Vertex3D* findVertex(const GEOMHANDLE vertex) const;
    
    /**
     * @brief Finds all edges whose start or end vertices reference the given vertex handle.
     * @param outEdges QList of edges to be filled.
     * @param vertex Handle of vertex to search for.
     */
    void findRelatedEdgesForVertex(QList<GEOMHANDLE>& outEdges, GEOMHANDLE vertex);
    
    /**
     * @brief Finds all edges whose start or end vertices reference any of the given vertex handles.
     * @param outEdges QList of edges to be filled.
     * @param vertices List of vertex handles to search for. If an edge matches any of these it is added to the output list.
     */
    void findRelatedEdgesForVertex(QList<GEOMHANDLE>& outEdges, QList<GEOMHANDLE>& vertices);
    
    /**
     * @brief Finds all faces whose edges reference the given vertex handle.
     * @param outFaces QList of faces to be filled.
     * @param vertex Handle of vertex to search for.
     */
    void findRelatedFacesForVertex(QList<GEOMHANDLE>& outFaces, GEOMHANDLE vertex);
    
    /**
     * @brief Finds all faces whose edges reference any of the given vertex handles.
     * @param outFaces QList of faces to be filled.
     * @param vertices List of vertex handles to search for. If an edge matches any of these it is added to the output list.
     */
    void findRelatedFacesForVertex(QList<GEOMHANDLE>& outFaces, QList<GEOMHANDLE>& vertices);
    
    // ===== End vertex functions ===== \\
    
    // ===== Begin edge functions ===== \\
    
    /**
     * @brief Adds the specified edge.
     *
     * The edge's handles will be updated to reflect its place in the solid.
     * @param edge Edge to add.
     * @return True if addition was successful, false if it was not.
     */
    bool addEdge(Edge3D* edge);
    
    /**
     * @brief Removes the edge with the specified handle.
     *
     * Faces using this edge, or vertices part of this edge, are NOT updated about its removal.
     * @param edge Handle to search for.
     * @return Edge removed, or NULL if the edge with the specified handle was not found.
     */
    Edge3D* removeEdgeHandle(const GEOMHANDLE edge);
    
    /**
     * @brief Returns a pointer to the edge with the specified handle.
     * @param edge Handle of edge to find.
     * @return Pointer to edge, or NULL if not found.
     */
    Edge3D* findEdge(const GEOMHANDLE edge) const;
    
    /**
     * @brief Finds all faces which contain the given edge
     * @param outFaces QList of faces to be filled.
     * @param edge Handle of edge to search for.
     */
    void findRelatedFacesForEdge(QList<GEOMHANDLE>& outFaces, GEOMHANDLE edge);
    
    /**
     * @brief Finds all faces which contain any of the given edges.
     * @param outFaces QList of faces to be filled.
     * @param edges List of edge handles to search for. If an edge matches any of these it is added to the output list.
     */
    void findRelatedFacesForEdge(QList<GEOMHANDLE>& outFaces, QList<GEOMHANDLE>& edges);
    
    // ===== End edge functions ===== \\
    
    // ===== Begin face functions ===== \\
    
    /**
     * @brief Adds the specified face.
     *
     * The face's handles will be updated to reflect its place in the solid.
     * @param face Face to add.
     * @return True if addition was successful, false if it was not.
     */
    bool addFace(Face3D* face);
    
    /**
     * @brief Removes the face with the specified handle.
     *
     * Vertices or edges part of this face are NOT updated about its removal.
     * @param face Handle to search for.
     * @return Face removed, or NULL if the face with the specified handle was not found.
     */
    Face3D* removeFaceHandle(const GEOMHANDLE face);
    
    /**
     * @brief Returns a pointer to the face with the specified handle.
     * @param face Handle of face to find.
     * @return Pointer to face, or NULL if not found.
     */
    Face3D* findFace(const GEOMHANDLE face) const;
    
    // ===== End face functions ===== \\

    // TODO: condenseFaces function: should check all faces to see whether any
    // adjacent faces are coplanar, and output a list of faces where the coplanar
    // faces are combined (ie. they are replaced by one single face which uses all
    // the edgs of the previous faces).
    
    /**
     * @brief Outputs information about the given geometry component to the GeomInfo struct.
     *
     * If the geometry component was not found, the info struct will be set to GeomType::Null.
     * @param handle Handle to search for.
     * @param info Info struct to fill out.
     */
    void identifyHandle(GEOMHANDLE handle, GeomInfo& info) const;
    
    /**
     * @brief Returns the vertex list.
     * @return List of all current vertices.
     */
    const Vertex3DList& getVertexList() const { return m_Vertices; }
    
    /**
     * @brief Returns the edge list.
     * @return List of all current edges.
     */
    const Edge3DList& getEdgeList() const { return m_Edges; }
    
    /**
     * @brief Returns the face list.
     * @return List of all current face.
     */
    const Face3DList& getFaceList() const { return m_Faces; }
    
    /**
     * @brief Returns this solid's handle.
     * @return Handle of the solid.
     */
    inline GEOMHANDLE getHandle() const { return m_hHandle; }
    
    /**
     * @brief Sets this solid's handle.
     * @param handle Value to set.
     */
    inline void setHandle(const GEOMHANDLE handle);

private:
    /**
     * @brief Returns whether we can add a new vertex.
     * @return True if there is space for another vertex, false otherwise.
     */
    bool canAddVertex() { return m_Vertices.size() < MAX_VERTICES; }
    
    /**
     * @brief Returns whether we can add a new edge.
     * @return True if there is space for another edge, false otherwise.
     */
    bool canAddEdge() { return m_Edges.size() < MAX_EDGES; }
    
    /**
     * @brief Returns whether we can add a new face.
     * @return True if there is space for another face, false otherwise.
     */
    bool canAddFace() { return m_Faces.size() < MAX_FACES; }

    // Handles
    VBO_OFFSET      m_hVBOHandle;       /**< Offset from the beginning of the VBO that this solid's vertices can be found at. */
    GEOMHANDLE      m_hHandle;          /**< Geometry handle for this solid. */
    IndexPool       m_IndexPool;        /**< Index manager for vertices. */
    
    Vertex3DList    m_Vertices;         /**< List of vertices in this solid. */
    Edge3DList      m_Edges;            /**< List of edges in this solid. */
    Face3DList      m_Faces;            /**< List of faces in this solid. */
};

#endif // SOLID_H
