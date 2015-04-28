#ifndef RENDERENTRYMAPBRUSH_H
#define RENDERENTRYMAPBRUSH_H

#include "rendertableentry.h"
#include <QGeometryData>
#include "mapbrush.h"
#include "mapface.h"
#include <QGLMaterialCollection>
#include <QSharedPointer>

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT RenderEntryMapBrush : public RenderTableEntry
{
    Q_OBJECT
public:
    RenderEntryMapBrush(MODEL_NAMESPACE::MapBrush* component, MapDocumentView* parent = 0);
    virtual ~RenderEntryMapBrush();

    inline MODEL_NAMESPACE::MapBrush* brush() const
    {
        return qobject_cast<MODEL_NAMESPACE::MapBrush*>(component());
    }

    virtual bool isValid() const { return true; }
    
    virtual QMatrix4x4 transformMatrix() const;

protected:
    virtual QGLSceneNode* sceneNodeFor(IRender::RenderMode mode) const;
    virtual bool drawDelegate(QGLPainter *painter, RenderMode mode, const QSize &viewSize, bool selection);

private:
    QGLSceneNode* buildWireframeNode() const;
    QGLSceneNode* buildSolidNode() const;
    static QGLSceneNode* buildTexturePlaneNode(const MODEL_NAMESPACE::MapFace* face, QSharedPointer<QGLMaterialCollection> collection);
    static QGeometryData edges(const MODEL_NAMESPACE::MapBrush* brush, const QColor &col);
    static QGeometryData faceData(const MODEL_NAMESPACE::MapFace *face, const QColor &col = QColor(), bool flipFaces = false);
    void drawWireframe(QGLPainter* painter, const QSize &size, bool forSelection);
    static QGeometryData brushVerticesToGeometryData(MODEL_NAMESPACE::MapBrush* brush);
    static QVector2D cheapTexCoordFor(const QVector3D &vertex, const QVector3D &faceNormal);
    static void addTextureCoordinates(const MODEL_NAMESPACE::MapFace* face, QGeometryData &data);
    static void handleFaceTexture(const MODEL_NAMESPACE::MapFace* face, QSharedPointer<QGLMaterialCollection> collection, QGLSceneNode* node, const QColor &col = QColor(255,255,255));
};

VIEW_END_NAMESPACE

#endif // RENDERENTRYMAPBRUSH_H
