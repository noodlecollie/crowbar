#include "renderentrytoolbasicsquare.h"
#include <QGLSceneNode>
#include <QtDebug>

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

RenderEntryToolBasicSquare::RenderEntryToolBasicSquare(ToolBasicSquare *component, MapDocumentView *parent) :
    RenderTableEntry(component, parent)
{
}

RenderEntryToolBasicSquare::~RenderEntryToolBasicSquare()
{
}

QGLSceneNode* RenderEntryToolBasicSquare::sceneNodeFor(IRender::RenderMode mode) const
{
    switch ( mode )
    {
    case IRender::Triangles:
        return (square()->visibleRenderModes() & ToolComponent::Triangles) == ToolComponent::Triangles
                ? buildSceneNode()
                : NULL;

    case IRender::Edges:
        return (square()->visibleRenderModes() & ToolComponent::Edges) == ToolComponent::Edges
                ? buildSceneNode()
                : NULL;

    default:
        return NULL;
    }
}

QGeometryData RenderEntryToolBasicSquare::innerFace() const
{
    Q_ASSERT(square());

    float max = square()->size()/2.0f;
    float min = -max;
    QColor col = square()->color();

    QVector3D ll(min, min, 0);
    QVector3D ul(min, max, 0);
    QVector3D ur(max, max, 0);
    QVector3D lr(max, min, 0);

    QGeometryData data;
    data.appendVertex(lr, ur, ul, ll);
    data.appendColor(col, col, col, col);

    data.appendIndex(0);
    data.appendIndex(1);
    data.appendIndex(2);
    data.appendIndex(3);

    return data;
}

QGeometryData RenderEntryToolBasicSquare::edges() const
{
    Q_ASSERT(square());

    float max = square()->size()/2.0f;
    float min = -max;
    QColor col = square()->borderColor();

    QVector3D ll(min, min, 0.1f);
    QVector3D ul(min, max, 0.1f);
    QVector3D ur(max, max, 0.1f);
    QVector3D lr(max, min, 0.1f);

    QGeometryData data;
    data.appendVertex(lr, ur, ul, ll);
    data.appendColor(col, col, col, col);

    data.appendIndex(0);
    data.appendIndex(1);
    data.appendIndex(1);
    data.appendIndex(2);
    data.appendIndex(2);
    data.appendIndex(3);
    data.appendIndex(3);
    data.appendIndex(0);

    return data;
}

QGLSceneNode* RenderEntryToolBasicSquare::buildSceneNode() const
{
    QGLSceneNode* inner = new QGLSceneNode();
    inner->setGeometry(innerFace());
    inner->setDrawingMode(QGL::TriangleFan);
    inner->setStart(0);
    inner->setCount(inner->geometry().indexCount());
    inner->setEffect(QGL::FlatPerVertexColor);

    QGLSceneNode* outer = new QGLSceneNode();
    outer->setGeometry(edges());
    outer->setDrawingMode(QGL::Lines);
    outer->setStart(0);
    outer->setCount(outer->geometry().indexCount());
    outer->setEffect(QGL::FlatPerVertexColor);
    outer->setDrawingWidth(square()->borderWidth());

    QGLSceneNode* wrapper = new QGLSceneNode();
    wrapper->addNode(inner);
    wrapper->addNode(outer);

    return wrapper;
}

VIEW_END_NAMESPACE
