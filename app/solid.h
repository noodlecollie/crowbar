/** \file solid.h
 * \brief TODO
 */

/* Notes:
 * - When creating a new solid from scratch, it's best to construct all required vertices first before moving on to edges and then faces.
 */

#ifndef SOLID_H
#define SOLID_H

#include <QVector>
#include "face.h"
#include "indexpool.h"

struct GeomInfo
{
public:
    enum GeomType
    {
        Null = 0,
        Vertex,
        Edge,
        Face,
        Solid
    };
    
    GEOMHANDLE  m_hHandle;
    GeomType    m_iType;
    GEOMHANDLE  m_hParentSolid; // Null if m_iType is Solid.
};

class Solid3D : public GeomMetaHandle
{
public:
    typedef QVector<Vertex3D*> Vertex3DList;
    typedef QVector<Edge3D*> Edge3DList;
    typedef QVector<Face3D*> Face3DList;
    
    enum
    {
        MAX_VERTICES = 384,
        MAX_EDGES = 384,
        MAX_FACES = 384
    };

    Solid3D();
    ~Solid3D();
    
    // ===== Begin vertex functions ===== \\
    
    // Adds the specified vertex. The vertex's handles will be updated to reflect its place in the solid.
    // Returns true if the vertex could be added, false if not.
    // Edges/faces using this vertex are NOT updated about its removal.
    bool addVertex(Vertex3D* vertex);
    
    // Removes the vertex with the specified handle.
    // The vector will be shrunk and other vertices' VBO handles
    // updated. The removed vertex's parent handle will be set to 0.
    Vertex3D* removeVertexHandle(const GEOMHANDLE vertex);
    
    // Returns a pointer to the vertex with the specified handle, or NULL if not found.
    Vertex3D* findVertex(const GEOMHANDLE vertex) const;
    
    void findRelatedEdgesForVertex(QList<GEOMHANDLE>& outEdges, GEOMHANDLE vertex);
    void findRelatedEdgesForVertex(QList<GEOMHANDLE>& outEdges, QList<GEOMHANDLE>& vertices);
    void findRelatedFacesForVertex(QList<GEOMHANDLE>& outFaces, GEOMHANDLE vertex);
    void findRelatedFacesForVertex(QList<GEOMHANDLE>& outFaces, QList<GEOMHANDLE>& vertices);
    
    // ===== End vertex functions ===== \\
    
    // ===== Begin edge functions ===== \\
    
    bool addEdge(Edge3D* edge);
    Edge3D* removeEdgeHandle(const GEOMHANDLE edge);
    Edge3D* findEdge(const GEOMHANDLE edge) const;
    void findRelatedFacesForEdge(QList<GEOMHANDLE>& outFaces, GEOMHANDLE edge);
    void findRelatedFacesForEdge(QList<GEOMHANDLE>& outFaces, QList<GEOMHANDLE>& edges);
    
    // ===== End edge functions ===== \\
    
    // ===== Begin face functions ===== \\
    
    bool addFace(Face3D* face);
    Face3D* removeFaceHandle(const GEOMHANDLE face);
    Face3D* findFace(const GEOMHANDLE face) const;
    
    // ===== End face functions ===== \\

    // TODO: condenseFaces function: should check all faces to see whether any
    // adjacent faces are coplanar, and output a list of faces where the coplanar
    // faces are combined (ie. they are replaced by one single face which uses all
    // the edgs of the previous faces).
    
    void identifyHandle(GEOMHANDLE handle, GeomInfo& info) const;
    
    // Returns vertex list
    const Vertex3DList& getVertexList() const { return m_Vertices; }
    
    // Returns edge list
    const Edge3DList& getEdgeList() const { return m_Edges; }
    
    // Returns face list
    const Face3DList& getFaceList() const { return m_Faces; }
    
    inline GEOMHANDLE getHandle() const { return m_hHandle; }
    inline void setHandle(const GEOMHANDLE handle);

private:
    bool canAddVertex() { return m_Vertices.size() < MAX_VERTICES; }

    // Handles
    VBO_OFFSET      m_hVBOHandle;       /**< Offset from the beginning of the VBO that this solid's vertices can be found at. */
    GEOMHANDLE      m_hHandle;
    IndexPool       m_IndexPool;
    
    Vertex3DList    m_Vertices;
    Edge3DList      m_Edges;
    Face3DList      m_Faces;
};

#endif // SOLID_H
