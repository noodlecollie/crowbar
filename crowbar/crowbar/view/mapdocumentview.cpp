#include "mapdocumentview.h"
#include <QGLSceneNode>
#include "mapdocument.h"
#include "mapcomponent.h"
#include "rendertableentries/renderentryfactory.h"
#include "mapworld.h"
#include <QGLPainter>
#include <QtDebug>
#include <QByteArray>
#include "mapview.h"
#include <QGLLightParameters>
#include <QGraphicsBillboardTransform>
#include "view_util.h"
#include <QGLLightModel>
#include "mapface.h"
#include "mapbrush.h"

using namespace MODEL_NAMESPACE;

// Weird things are happening with these and I don't know why.
#define DISABLE_DRAW_SELECTION_BOUNDS

VIEW_BEGIN_NAMESPACE

MapDocumentView::MapDocumentView(MapDocument *document, QObject *parent) :
    QObject(parent)
{
    m_pLightModel = new QGLLightModel(this);
    m_pLightModel->setAmbientSceneColor(QColor::fromRgbF(0.8f, 0.8f, 0.8f));
    m_pSelectedBoundsNode = NULL;

    Q_ASSERT(document);
    m_pDocument = document;
    populateFromDocument();
    connect(document, &MapDocument::componentAdded, this, &MapDocumentView::documentComponentAdded);
    connect(document, &MapDocument::componentRemoved, this, &MapDocumentView::documentComponentRemoved);
}

MapDocumentView::~MapDocumentView()
{
    delete m_pSelectedBoundsNode;
    m_pSelectedBoundsNode = NULL;
}

MapDocument* MapDocumentView::document() const
{
    return m_pDocument.data();
}

bool MapDocumentView::contains(MapComponent *component) const
{
    return m_RenderTable.contains(component);
}

QGLSceneNode* MapDocumentView::sceneNode(MapComponent *component, IRender::RenderMode mode) const
{
    RenderTableEntry* entry = m_RenderTable.value(component);
    if ( !entry ) return NULL;

    return entry->sceneNode(mode);
}

void MapDocumentView::populateFromDocument()
{
    if ( !document() )
    {
        clearAll();
        return;
    }

    // Go through the document tree and add has table entries for each component.
    
    MapWorld* world = document()->world();
    
    // For now this is recursive - hopefully the stack won't overflow, but there's a possibility
    // we may have to make this iterative if things do go bad...
    populateRecursive(world);
}

void MapDocumentView::updateRenderEntryFor(MapComponent *component)
{
    // Check to see if an entry exists. If it doesn't, try to create it.
    RenderTableEntry* entry = m_RenderTable.value(component, NULL);
    if ( !entry )
    {
        entry = RenderTableEntries::entryFor(component);
        if ( entry->isValid() )
        {
            entry->setParentView(this);
            m_RenderTable.insert(component, entry);
        }
        else
        {
            delete entry;   // <- Don't forget me!!
            entry = NULL;
        }
    }

    if ( !entry ) return;

    // Update all data for the entry.
    entry->rebuildAllSceneNodes();
}

void MapDocumentView::populateRecursive(MapComponent *c)
{
    updateRenderEntryFor(c);
    
    // Add each child to the table.
    QList<MapComponent*> children = c->childComponents();
    foreach ( MapComponent* child, children )
    {
        populateRecursive(child);
    }
}

void MapDocumentView::clearAll()
{
    for ( RenderTable::iterator it = m_RenderTable.begin(); it != m_RenderTable.end(); ++it )
    {
        delete it.value();
    }
    m_RenderTable.clear();
}

void MapDocumentView::handleComponentDeleted(MapComponent *component)
{
    RenderTableEntry* entry = m_RenderTable.value(component, NULL);
    if ( entry ) m_RenderTable.remove(component);
    entry->deleteLater();
}

void MapDocumentView::draw(QGLPainter *painter, RenderMode mode, const QSize &viewSize)
{
    Q_ASSERT(document());

    // Draw recursively. Again, if we get stack overflows we'll have to make this iterative.
    drawRecursive(document()->world(), painter, mode, viewSize);
    drawSelectedBounds(painter);
}

void MapDocumentView::drawRecursive(MapComponent *component, QGLPainter *painter, RenderMode mode, const QSize &viewSize,
                                    ComponentSelectionManager* selection)
{
    // TODO: Make me better. For now, we save the matrix of the scene node on the QGLPainter so that it'll apply
    // to our chilren - we have to do this manually because the scene nodes aren't explicitly parented to each other.
    // Basically, we need a better renderer but that'll be the third re-write and I'm done right now.

    RenderTableEntry* entry = m_RenderTable.value(component, NULL);
    QMatrix4x4 mat = entry ? entry->transformMatrix() : component->hierarchyTransform()->transformMatrix();
    
    bool mvtransformed = false;
    bool pjtransformed = false;
    if ( !mat.isIdentity() || component->billboard() )
    {
        if ( !component->billboard() )
        {
            painter->modelViewMatrix().push();
            painter->modelViewMatrix() *= mat;
        }
        else
        {
            QMatrix4x4 current = painter->modelViewMatrix().top();
            current *= mat;
            QGraphicsBillboardTransform bbtr;
            bbtr.applyTo(&current);

            painter->modelViewMatrix().push();
            painter->modelViewMatrix().setToIdentity();
            painter->modelViewMatrix() *= current;
            
            if ( component->fixedBillboard() )
            {
                const QMatrix4x4 &m = painter->projectionMatrix().top();
                QMatrix4x4 temp;
                temp(0,0) = 1.0f/m(0,0);
                temp(1,1) = 1.0f/m(1,1);
                
                painter->projectionMatrix().push();
                painter->projectionMatrix() *= temp;
                
                pjtransformed = true;
            }
        }

        mvtransformed = true;
    }

    if ( entry )
    {
        QGLSceneNode* node = entry->sceneNode(mode);
        if ( node )
        {
            // This isn't as efficient as utilising a whole tree of scene nodes with recursive culling,
            // but it's better than nothing.
            QBox3D bbox = node->boundingBox();
            bool finite = bbox.isFinite();
            bool nullbox = bbox.isNull();
            bool culled = painter->isCullable(bbox);

            if ( !finite || nullbox || !culled )
            {
                if ( selection )
                {
                    entry->draw(painter, mode, viewSize, true);
                    selection->compareValues(painter, component);
                }
                else
                {
                    entry->draw(painter, mode, viewSize);
                }
            }
        }
    }

    // If the component is hidden, terminate drawing at this point.
    if ( component->hidden() )
    {
        if ( mvtransformed ) painter->modelViewMatrix().pop();
        if ( pjtransformed ) painter->projectionMatrix().pop();
        return;
    }

    // Call for all children of the component.
    QList<MapComponent*> children = component->childComponents();
    foreach ( MapComponent* c, children )
    {
        drawRecursive(c, painter, mode, viewSize, selection);
    }

    if ( mvtransformed ) painter->modelViewMatrix().pop();
    if ( pjtransformed ) painter->projectionMatrix().pop();
}

QList<MapComponent*> MapDocumentView::objectsWithin(const QRect &selection, QGLPainter *painter, RenderMode mode,
                                                    const QSize &size)
{
    // Fix up the selection rectangle - OpenGL origin is bottom left!
    int bottomLeft = selection.bottomLeft().y() + 1;
    int newY = size.height() - 1 - bottomLeft;
    QRect newRect(selection.x(), newY, selection.width() + 1, selection.height() + 1);

    ComponentSelectionManager sel(newRect, painter);
    drawRecursive(document()->world(), painter, mode, size, &sel);
    return sel.objects();
}

MapFace* MapDocumentView::faceAt(const QPoint &point, QGLPainter *painter, RenderMode mode,
                                                  const QSize &size)
{
    // Firstly get the list of objects at the point.
    QList<MapComponent*> objects = objectsWithin(QRect(point, point), painter, mode, size);

    // If no objects, return null.
    if ( objects.count() < 1 ) return NULL;

    // Check if the object we got was a brush.
    MapBrush* brush = qobject_cast<MapBrush*>(objects.at(0));
    if ( !brush ) return NULL;

    // Check the brush has a render entry;
    RenderTableEntry* entry = m_RenderTable.value(brush, NULL);
    if ( !entry ) return NULL;

    // Check the render data is valid;
    QGLSceneNode* node = entry->sceneNode(mode);
    if ( !node ) return NULL;

    QList<QGLSceneNode*> faces = node->children().at(0)->children();
    if ( faces.count() < 1 ) return NULL;

    // Get the transform to put the faces into world space.
    QMatrix4x4 transform = brush->hierarchyTransform()->globalTransformMatrix();

    int bottomLeft = point.y() + 1;
    int newY = size.height() - 1 - bottomLeft;
    QRect newRect(point.x(), newY, 1, 1);

    // Create a new selection manager for the comparisons.
    FaceSelectionManager sel(newRect, painter);

    // The painter should already be cleared by the selection manager.
    // For each face, draw it and query the selection manager.

    for ( int i = 0; i < faces.count(); i++ )
    {
        QGLSceneNode* face = faces.at(i);
        QMatrix4x4 localTransform = face->localTransform();
        face->setLocalTransform(transform);
        face->draw(painter);
        face->setLocalTransform(localTransform);
        sel.compareValues(painter, brush->faces(i));
    }

    // Get the "list" of faces (there should be maximum 1).
    QList<MapFace*> faceList = sel.objects();

    // Return the face if it was picked.
    return faceList.count() > 0 ? faceList.at(0) : NULL;
}

int MapDocumentView::count() const
{
    return m_RenderTable.count();
}

void MapDocumentView::documentComponentAdded(MapComponent *component)
{
    updateRenderEntryFor(component);
}

void MapDocumentView::documentComponentRemoved(MapComponent *component)
{
    removeRenderEntryFor(component);
}

void MapDocumentView::removeRenderEntryFor(MapComponent *component)
{
    delete m_RenderTable.take(component);
}

QList<const QGLLightParameters*> MapDocumentView::lights() const
{
    if ( !document() ) return QList<const QGLLightParameters*>();
    
    return document()->lightParameters();
}

QGLLightModel* MapDocumentView::lightModel()
{
    return m_pLightModel;
}

const QGLLightModel* MapDocumentView::lightModel() const
{
    return m_pLightModel;
}

QBox3D MapDocumentView:: selectionBounds() const
{
    return document()->selectedSet().globalBoundingBox();
}

void MapDocumentView::drawSelectedBounds(QGLPainter *painter)
{
#ifndef DISABLE_DRAW_SELECTION_BOUNDS
    QBox3D bounds = selectionBounds();
    if ( bounds.isInfinite() || bounds.isNull() )
    {
        if ( m_pSelectedBoundsNode ) delete m_pSelectedBoundsNode;
        m_pSelectedBoundsNode = NULL;
        return;
    }
    
    if ( !m_pSelectedBoundsNode || bounds != m_SelectedBounds )
    {
        if ( m_pSelectedBoundsNode ) delete m_pSelectedBoundsNode;
        m_pSelectedBoundsNode = View_Util::wireframeBox(bounds, QColor(255,117,43));
        m_pSelectedBoundsNode->setEffect(QGL::FlatPerVertexColor);
        m_SelectedBounds = bounds;
    }
    
    m_pSelectedBoundsNode->draw(painter);
#else
    Q_UNUSED(painter);
#endif
}

VIEW_END_NAMESPACE
