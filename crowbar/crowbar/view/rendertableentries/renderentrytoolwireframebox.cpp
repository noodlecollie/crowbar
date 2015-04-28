#include "renderentrytoolwireframebox.h"
#include "view_util.h"
#include <QGLSceneNode>
#include "vieweffects.h"

VIEW_BEGIN_NAMESPACE

RenderEntryToolWireframeBox::RenderEntryToolWireframeBox(Model::ToolWireframeBox *component, MapDocumentView *parent) :
    RenderTableEntry(component, parent)
{
}

RenderEntryToolWireframeBox::~RenderEntryToolWireframeBox()
{
}

QGLSceneNode* RenderEntryToolWireframeBox::sceneNodeFor(IRender::RenderMode mode) const
{
    return buildNode(mode);
}

QGLSceneNode* RenderEntryToolWireframeBox::buildNode(IRender::RenderMode mode) const
{
    QBox3D b = box()->box();
    if ( b.isInfinite() || b.isNull() ) return new QGLSceneNode();

    QGLSceneNode* node = View_Util::wireframeBox(b, box()->color());

    if ( mode == IRender::Edges )
    {
        node->setUserEffect(Effects::presetEffect(Effects::SelectionBox));
    }
    else
    {
        node->setEffect(QGL::FlatPerVertexColor);
    }

    return node;
}

VIEW_END_NAMESPACE
