#ifndef MAPBRUSHRENDERPROXY2_H
#define MAPBRUSHRENDERPROXY2_H

#include "view_global.h"
#include <QObject>
#include "mapbrush.h"
#include <QPointer>
#include "irender.h"
#include <QGeometryData>
#include <QList>

#define EDGE_CENTROID_PADDING_PIXELS    1

class QGLSceneNode;

VIEW_BEGIN_NAMESPACE

class MapDocumentRenderer2;

class VIEWSHARED_EXPORT MapBrushRenderProxy2 : public QObject, public IRender
{
    Q_OBJECT
    Q_PROPERTY(const MODEL_NAMESPACE::MapBrush brush READ brush)
public:
    explicit MapBrushRenderProxy2(const MODEL_NAMESPACE::MapBrush* brush, QObject *parent = 0);

    // Is true if the proxy points to a valid brush.
    bool isValid() const;

    const MODEL_NAMESPACE::MapBrush* brush() const;

    virtual void draw(QGLPainter *painter, RenderMode mode, const QSize &viewSize);

    MapDocumentRenderer2*   parentRenderer();
    const MapDocumentRenderer2* parentRenderer() const;

signals:
    void renderDataDestroyed(MapBrushRenderProxy2*);

public slots:
    void updateRenderData(IRender::RenderMode mode);
    void updateRenderData();

private slots:
    void handleBrushDestroyed();

private:
    enum WireframeComponents
    {
        Geometry = 0,
        CentralPoint,

        TOTAL_COMPONENTS
    };

    typedef QList<QGLSceneNode*> SceneNodeList;

    QGeometryData faceData(const MODEL_NAMESPACE::MapFace* face) const;
    QGeometryData edgeData(const MODEL_NAMESPACE::MapBrush* brush) const;
    QGLSceneNode* centreMarker(const MODEL_NAMESPACE::MapBrush* brush, const QGeometryData &edges, float size) const;
    QGeometryData renderData(const MODEL_NAMESPACE::MapBrush* brush, IRender::RenderMode mode) const;
    QGLSceneNode* sceneNode(IRender::RenderMode mode, const MODEL_NAMESPACE::MapBrush* brush) const;

    void initialiseSceneNodeList();
    void drawWireframe(QGLPainter* painter, const QSize &size);

    QPointer<const MODEL_NAMESPACE::MapBrush>   m_pBrush;
    SceneNodeList                               m_SceneNodes;
};

VIEW_END_NAMESPACE

#endif // MAPBRUSHRENDERPROXY2_H
