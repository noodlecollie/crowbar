#ifndef MAPBRUSH_H
#define MAPBRUSH_H

#include "mapcomponent.h"
#include "mapvertex.h"
#include "mapedge.h"
#include "mapface.h"

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapBrush : public MapComponent
{
    Q_OBJECT
    MAPCOMPONENT_SUBCLASS(MapBrush, MapComponent)
public:
    typedef Vectors::ExposedOVector_MapVertex VertexVector;
    typedef Vectors::ExposedOVector_MapEdge EdgeVector;
    typedef Vectors::ExposedOVector_MapFace FaceVector;

//    explicit MapBrush(MapComponent *parent = 0);
//    virtual ~MapBrush();

    VertexVector& vertices();
    const VertexVector& vertices() const;
    MapVertex* vertices(int index);
    const MapVertex* vertices(int index) const;

    EdgeVector& edges();
    const EdgeVector& edges() const;
    MapEdge* edges(int index);
    const MapEdge* edges(int index) const;

    FaceVector& faces();
    const FaceVector& faces() const;
    MapFace* faces(int index);
    const MapFace* faces(int index) const;

    // Convenience methods for adding sub-components.
    // The brush owns the created component.
    MapVertex* newVertex(const QVector3D &pos = QVector3D(), const QColor &col = QColor(255,255,255));
    MapEdge* newEdge(MapVertex* v0 = NULL, MapVertex* v1 = NULL);
    MapFace* newFace();

    void deleteAll();
    
    virtual MapComponent* clone() const;
    void setColor(const QColor &col);
    
    inline MapBrush* proxyTargetBrush() const { return qobject_cast<MapBrush*>(proxyTarget()); }

    virtual QBox3D localBoundingBox() const;
    virtual QVector3D localCentroid() const;

    void setAllVerticesSelected(bool selected);
    void setAllEdgesSelected(bool selected);
    void setAllFacesSelected(bool selected);
    void setAllSubComponentsSelected(bool selected);

    // Translates the origin but keeps the vertices in the same positions relative
    // to the parent co-ordinate space.
    void translateOriginInPlace(const QVector3D &translation);
    void setOriginInPlace(const QVector3D &position);

    void translateVertices(const QVector3D &translation);

signals:

public slots:

private slots:
    void handleVertexAdded(MapVertex* v);
    void handleVertexRemoved(MapVertex* v);
    void handleEdgeAdded(MapEdge* e);
    void handleEdgeRemoved(MapEdge* e);
    void handleFaceAdded(MapFace* f);
    void handleFaceRemoved(MapFace* f);

    // If a component is deleted, remove it from the list.
    void detachVertex(QObject* v);
    void detachEdge(QObject* e);
    void detachFace(QObject* f);
    
protected:
    virtual void cloneProperties(MapComponent *c) const;

private:
    void connectSignals();

    VertexVector*   m_pVertices;
    EdgeVector*     m_pEdges;
    FaceVector*     m_pFaces;
};

#ifdef QT_DEBUG
void outputPrecisionInfo(const MapBrush &brush);
#endif

MODEL_END_NAMESPACE

#endif // MAPBRUSH_H
