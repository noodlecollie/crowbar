#ifndef RENDERTABLEENTRY_H
#define RENDERTABLEENTRY_H

#include <QObject>
#include "view_global.h"
#include <QVector>
#include "irender.h"
#include "model_global.h"
#include <QPointer>
#include <QGeometryData>

#define EDGE_CENTROID_PADDING_PIXELS    1
#define LINE_SELECTION_BLOAT            8.0f
#define POINT_SELECTION_BLOAT           10.0f
#define EDGE_CENTROID_SIZE              6.0f

class QGLSceneNode;

MODEL_BEGIN_NAMESPACE
class MapComponent;
MODEL_END_NAMESPACE

VIEW_BEGIN_NAMESPACE

class MapDocumentView;

// Holds render data for a given MapComponent in a render table.
// The table is indexed by MapComponent pointer and this class is held
// as the associated value.
class VIEWSHARED_EXPORT RenderTableEntry : public QObject, public IRender
{
    Q_OBJECT
public:
    RenderTableEntry(MODEL_NAMESPACE::MapComponent* component, MapDocumentView* parent = 0);
    virtual ~RenderTableEntry();

    QGLSceneNode* sceneNode(IRender::RenderMode mode) const;
    void setSceneNode(IRender::RenderMode mode, QGLSceneNode* node);

    MODEL_NAMESPACE::MapComponent* component() const;

    void buildSceneNode(IRender::RenderMode mode);
    void rebuildAllSceneNodes();

    MapDocumentView* parentView() const;
    void setParentView(MapDocumentView* parent);

    virtual void draw(QGLPainter *painter, RenderMode mode, const QSize &viewSize);
    void draw(QGLPainter *painter, RenderMode mode, const QSize &viewSize, bool selection);

    // Override me in subclasses where sceneNodeFor() is implemented.
    virtual bool isValid() const { return false; }
    
    // Override me in subclasses to perform preprocessing on the transform matrix.
    virtual QMatrix4x4 transformMatrix() const;

signals:
    void componentDeleted(MODEL_NAMESPACE::MapComponent*);

private slots:
    void componentRenderDataChanged();
    void componentHiddenStatusChanged();
    void componentDeleted();

protected:
    virtual QGLSceneNode* sceneNodeFor(IRender::RenderMode mode) const;
    virtual bool drawDelegate(QGLPainter* painter, RenderMode mode, const QSize &viewSize, bool selection);
    static QGLSceneNode* wireframeCentreMarker(const QGeometryData &edges, float size, const QColor &col);
    static QGLSceneNode* wireframeCentreMarker(const QVector3D &origin, float size, const QColor &col);
    
private:
    void updateSceneNodesIfRequired();
    void setNodeOptions(QGLSceneNode* node);

    QVector<QGLSceneNode*>                      m_SceneNodes;
    QPointer<MODEL_NAMESPACE::MapComponent> m_pComponent;
    bool    m_bSceneNodeUpdate;
};

VIEW_END_NAMESPACE

#endif // RENDERTABLEENTRY_H
