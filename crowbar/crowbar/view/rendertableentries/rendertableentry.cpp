#include "rendertableentry.h"
#include <QGLSceneNode>
#include "mapdocumentview.h"
#include "mapcomponent.h"
#include <QtDebug>

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

RenderTableEntry::RenderTableEntry(MapComponent *component, MapDocumentView *parent) :
    QObject(parent), m_SceneNodes(IRender::TOTAL_RENDER_MODES), m_pComponent(component)
{
    Q_ASSERT(component);
    connect(component, SIGNAL(renderDataChanged(ChangeId_t)), this, SLOT(componentRenderDataChanged()));
    connect(component, SIGNAL(hiddenStatusChanged(bool)), this, SLOT(componentHiddenStatusChanged()));
    connect(component, SIGNAL(destroyed()), this, SLOT(componentDeleted()));
}

RenderTableEntry::~RenderTableEntry()
{
    // Delete all the scene nodes we have stored.
    for ( int i = 0; i < m_SceneNodes.count(); i++ )
    {
        delete m_SceneNodes.at(i);
    }
    m_SceneNodes.clear();
}

QGLSceneNode* RenderTableEntry::sceneNode(IRender::RenderMode mode) const
{
    return m_SceneNodes.at((int)mode);
}

void RenderTableEntry::setSceneNode(IRender::RenderMode mode, QGLSceneNode *node)
{
    m_SceneNodes.replace((int)mode, node);
}

MapComponent* RenderTableEntry::component() const
{
    return m_pComponent.data();
}

void RenderTableEntry::rebuildAllSceneNodes()
{
    // Assumptions!
    for ( int i = 0; i < (int)IRender::TOTAL_RENDER_MODES; i++ )
    {
        buildSceneNode((IRender::RenderMode)i);
    }
}

QGLSceneNode* RenderTableEntry::sceneNodeFor(IRender::RenderMode mode) const
{
    // Nothing here! Subclasses re-implement me.
    Q_UNUSED(mode);
    Q_ASSERT(false);
    return NULL;
}

void RenderTableEntry::buildSceneNode(IRender::RenderMode mode)
{
    QGLSceneNode* node = sceneNodeFor(mode);
    delete sceneNode(mode);                     // FIX: Whoopsie
    setSceneNode(mode, node);
    if ( !node ) return;

    setNodeOptions(node);
}

MapDocumentView* RenderTableEntry::parentView() const
{
    return qobject_cast<MapDocumentView*>(parent());
}

void RenderTableEntry::setParentView(MapDocumentView *parent)
{
    setParent(parent);
}

void RenderTableEntry::componentRenderDataChanged()
{
    m_bSceneNodeUpdate = true;
}

void RenderTableEntry::updateSceneNodesIfRequired()
{
    if ( m_bSceneNodeUpdate )
    {
        rebuildAllSceneNodes();
        m_bSceneNodeUpdate = false;
    }
}

void RenderTableEntry::draw(QGLPainter *painter, RenderMode mode, const QSize &viewSize)
{
    draw(painter, mode, viewSize, false);
}

void RenderTableEntry::draw(QGLPainter *painter, RenderMode mode, const QSize &viewSize, bool selection)
{
    Q_UNUSED(viewSize);

    updateSceneNodesIfRequired();
    
    // Firstly see if the drawing has been handled by the subclass.
    if ( drawDelegate(painter, mode, viewSize, selection) ) return;
    
    // If not, draw normally.
    QGLSceneNode* node = sceneNode(mode);
    if ( !node ) return;

    node->draw(painter);
}

bool RenderTableEntry::drawDelegate(QGLPainter *painter, RenderMode mode, const QSize &viewSize, bool selection)
{
    Q_UNUSED(painter);
    Q_UNUSED(mode);
    Q_UNUSED(viewSize);
    Q_UNUSED(selection);
    
    // We don't do anything here.
    return false;
}

void RenderTableEntry::componentHiddenStatusChanged()
{
    for ( int i = 0; i < (int)IRender::TOTAL_RENDER_MODES; i++ )
    {
        QGLSceneNode* node = sceneNodeFor((IRender::RenderMode)i);
        if ( !node ) continue;

        setNodeOptions(node);
    }
}

void RenderTableEntry::setNodeOptions(QGLSceneNode *node)
{
    node->setOption(QGLSceneNode::HideNode, component()->hidden());
}

void RenderTableEntry::componentDeleted()
{
    emit componentDeleted(component());
}

QMatrix4x4 RenderTableEntry::transformMatrix() const
{
    return component()->hierarchyTransform()->transformMatrix();
}

QGLSceneNode* RenderTableEntry::wireframeCentreMarker(const QGeometryData &edges, float size, const QColor &col)
{
    return wireframeCentreMarker(edges.count() > 0 ? edges.center() : QVector3D(0,0,0), size, col);
}

QGLSceneNode* RenderTableEntry::wireframeCentreMarker(const QVector3D &origin, float size, const QColor &col)
{
    // Create a new geometry container with the point.
    QGeometryData data;
    data.appendVertex(origin);
    data.appendColor(col);
    data.appendIndex(0);

    // Create the node.
    QGLSceneNode* node = new QGLSceneNode();
    node->setGeometry(data);
    node->setCount(data.count());
    node->setDrawingMode(QGL::Points);
    node->setDrawingWidth(size);
    node->setEffect(QGL::FlatPerVertexColor);
    return node;
}

VIEW_END_NAMESPACE
