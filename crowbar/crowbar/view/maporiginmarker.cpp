#include "maporiginmarker.h"
#include <QGLBuilder>
#include <QGraphicsRotation3D>
#include <QGLSceneNode>
#include "view_util.h"

VIEW_BEGIN_NAMESPACE

MapOriginMarker::MapOriginMarker(QObject *parent) :
    QObject(parent)
{
}

QGLSceneNode* MapOriginMarker::sceneNode(IRender::RenderMode mode) const
{
    // We're always wireframe so we don't care about the mode.
    Q_UNUSED(mode);

    return View_Util::sceneNodeForWireframe(originData());
}

QGeometryData MapOriginMarker::originData()
{
    QGeometryData data;

    data.appendVertex(QVector3D(0,0,0), QVector3D(64,0,0));
    data.appendColor(QColor(255,0,0), QColor(255,0,0));
    data.appendIndex(0);
    data.appendIndex(1);

    data.appendVertex(QVector3D(0,0,0), QVector3D(0,64,0));
    data.appendColor(QColor(0,255,0), QColor(0,255,0));
    data.appendIndex(2);
    data.appendIndex(3);

    data.appendVertex(QVector3D(0,0,0), QVector3D(0,0,64));
    data.appendColor(QColor(0,0,255), QColor(0,0,255));
    data.appendIndex(4);
    data.appendIndex(5);

    return data;
}

VIEW_END_NAMESPACE
