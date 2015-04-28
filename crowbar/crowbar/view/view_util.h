#ifndef VIEW_UTIL_H
#define VIEW_UTIL_H

#include "view_global.h"
#include <QGeometryData>
#include <QBox3D>
#include <QColor>
#include <QMatrix4x4>
#include "facetextureinfo.h"
#include <QList>

class QGLSceneNode;

VIEW_BEGIN_NAMESPACE

namespace View_Util
{
    // Creates a scene node that displays the given geometry data as wireframe.
    // By default, the effect used is FlatPerVertexColor.
    VIEWSHARED_EXPORT QGLSceneNode* sceneNodeForWireframe(const QGeometryData &data);
    
    VIEWSHARED_EXPORT QGLSceneNode* solidBox(const QBox3D &box, const QColor &col);
    VIEWSHARED_EXPORT QGLSceneNode* wireframeBox(const QBox3D &box, const QColor &col);
    VIEWSHARED_EXPORT QGLSceneNode* simpleOriginMarker(float scale, const QVector3D position = QVector3D(0,0,0));
    VIEWSHARED_EXPORT QGLSceneNode* solidOriginMarker(float scale, const QVector3D position = QVector3D(0,0,0));
    
    VIEWSHARED_EXPORT QList<QVector2D> textureCoordinates(const MODEL_NAMESPACE::FaceTextureInfo &info, const QList<QVector3D> &vertices);
    VIEWSHARED_EXPORT QVector3D toPlaneSpace(const QVector3D &vec, const MODEL_NAMESPACE::FaceTextureInfo::TexturePlane &plane);
}

VIEW_END_NAMESPACE

#endif // VIEW_UTIL_H
