#ifndef MAPBRUSH_H
#define MAPBRUSH_H

#include "mapcomponent.h"
#include "model_global.h"
#include "imaphandle.h"
#include "handleobjectlist.h"
#include "mapvertex.h"
#include "mapedge.h"
#include "mapface.h"
#include "hierarchynode3d.h"

#define VERTICES_TITLE  "vertices"
#define EDGES_TITLE     "edges"
#define FACES_TITLE     "faces"

MODEL_BEGIN_NAMESPACE

class MapDocument;

class MODELSHARED_EXPORT MapBrush : public MapComponent, public IHierarchyNode3D
{
    Q_OBJECT
public:
    typedef HandleObjectList<MapVertex> MapVertexList;
    typedef HandleObjectList<MapEdge>   MapEdgeList;
    typedef HandleObjectList<MapFace>   MapFaceList;
    
    explicit MapBrush(MapDocument* document, QObject *parent = 0);
    ~MapBrush();
    
    virtual HierarchyNode3D& hierarchyNode();
    virtual const HierarchyNode3D& hierarchyNode() const;

    MapDocument* document();
    const MapDocument* document() const;
    void setDocument(MapDocument* doc);
    
    MapVertexList& vertices();
    const MapVertexList& vertices() const;
    
    MapEdgeList& edges();
    const MapEdgeList& edges() const;
    
    MapFaceList& faces();
    const MapFaceList& faces() const;
    
signals:
    // Emitted when something happens that would alter how the brush is shown in a view.
    void brushRenderDataChanged();
    
public slots:
    // The brush has been removed from the given document -
    // remove any internal links that rely on other objects within this document.
    void removedFromDocument(MapDocument* doc);

private slots:
    inline void sendBrushChanged() { emit brushRenderDataChanged(); }
    void handleVertexAdded(MODEL_NAMESPACE::MapVertex* v);
    void handleVertexRemoved(MODEL_NAMESPACE::MapVertex* v);
    void handleEdgeAdded(MODEL_NAMESPACE::MapEdge* e);
    void handleEdgeRemoved(MODEL_NAMESPACE::MapEdge* e);
    void handleFaceAdded(MODEL_NAMESPACE::MapFace* f);
    void handleFaceRemoved(MODEL_NAMESPACE::MapFace* f);
    void handleClearVertices();
    void handleClearEdges();
    void handleClearFaces();
    void fixUpVerticesForNewOrigin(const QVector3D &origin);
    void handleVectorTransformationChanged(const QVector3D &newVec);
    void handleSelectionChanged(bool selected);
    
private:
    MapVertexList       m_VertexList;
    MapEdgeList         m_EdgeList;
    MapFaceList         m_FaceList;
    HierarchyNode3D*    m_pHierarchyNode;
    MapDocument*        m_pDocument;
};

MODEL_END_NAMESPACE

QDebug MODELSHARED_EXPORT operator<<(QDebug dbg, const MODEL_NAMESPACE::MapBrush &brush);

#endif // MAPBRUSH_H
