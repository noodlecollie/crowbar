/* Notes:
 * - When creating a new solid from scratch, it's best to construct all required vertices first before moving on to edges and then faces.
 */

#ifndef SOLID_H
#define SOLID_H

#include <QVector>
#include "face.h"

class Solid3D : public GeomMetaHandle
{
public:
    typedef QVector<Vertex3D*> Vertex3DList;
    typedef QVector<Edge3D*> Edge3DList;
    typedef QVector<Face3D*> Face3DList;
    
    enum
    {
        MAX_VERTICES = 256,
        MAX_EDGES = 256
    };

    Solid3D();
    ~Solid3D();
    
    // ===== Begin vertex functions ===== \\
    
    // Adds the specified vertex. The vertex's handles will be updated.
    // Returns true if the vertex could be added, false if not.
    bool addVertex(Vertex3D* vertex);
    
    // Removes the vertex with the specified global handle.
    // The vector will be shrunk and other vertices' VBO handles
    // updated. The removed vertex's parent handle will be set to 0.
    Vertex3D* removeVertexHandle(const GEOMHANDLE vertex);
    
    // ===== End vertex functions ===== \\

    // TODO: condenseFaces function: should check all faces to see whether any
    // adjacent faces are coplanar, and output a list of faces where the coplanar
    // faces are combined (ie. they are replaced by one single face which uses all
    // the edgs of the previous faces).
    
    // Returns vertex list
    const Vertex3DList& getVertexList() const { return m_Vertices; }
    
    // Returns edge list
    const Edge3DList& getEdgeList() const { return m_Edges; }
    
    // Returns face list
    const Face3DList& getFaceList() const { return m_Faces; }

private:
    bool canAddVertex() { return m_Vertices.size() < 256; }

    // Handles
    GEOMHANDLE      m_hGlobalHandle;    /**< Global unique handle for this solid. */
    VBO_OFFSET      m_hVBOHandle;       /**< Offset from the beginning of the VBO that this solid's vertices can be found at. */
    
    Vertex3DList    m_Vertices;
    Edge3DList      m_Edges;
    Face3DList      m_Faces;
};

#endif // SOLID_H
