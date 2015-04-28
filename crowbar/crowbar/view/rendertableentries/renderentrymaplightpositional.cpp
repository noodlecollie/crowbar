#include "renderentrymaplightpositional.h"
#include "view_util.h"
#include <QGLSceneNode>

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

RenderEntryMapLightPositional::RenderEntryMapLightPositional(MapLightPositional *component, MapDocumentView *parent) :
    RenderTableEntry(component, parent)
{
}

RenderEntryMapLightPositional::~RenderEntryMapLightPositional()
{
}

QGLSceneNode* RenderEntryMapLightPositional::sceneNodeFor(RenderMode mode) const
{
    switch (mode)
    {
        case IRender::Edges:
        {
            return buildWireframeNode();
        }

        default:
        {
            return buildSolidNode();
        }
    }
}

QGLSceneNode* RenderEntryMapLightPositional::buildSolidNode() const
{
    QColor colour = light()->selected() ? QColor(255,0,0) : QColor(255,255,255);
    
    QGLSceneNode* node = View_Util::solidBox(light()->localBoundingBox(), colour);
    node->setEffect(QGL::LitMaterial);
    
    return node;
}

QGLSceneNode* RenderEntryMapLightPositional::buildWireframeNode() const
{
    QColor colour = light()->selected() ? QColor(255,0,0) : QColor(255,255,255);
    
    QGLSceneNode* node = View_Util::wireframeBox(light()->localBoundingBox(), colour);
    node->setEffect(QGL::FlatPerVertexColor);

    QGLSceneNode* centre = wireframeCentreMarker(node->geometry(), EDGE_CENTROID_SIZE, colour);

    QGLSceneNode* wrapper = new QGLSceneNode();
    wrapper->addNode(node);
    wrapper->addNode(centre);
    
    return wrapper;
}

VIEW_END_NAMESPACE
