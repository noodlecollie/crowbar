#include "mapbrush.h"
#include <QtDebug>
#include "mapdocument.h"

QDebug operator<<(QDebug dbg, const MODEL_NAMESPACE::MapBrush &brush)
{
    dbg.nospace() << &brush << " {" << brush.handle() << "}\nVertices:\n";
    
    if ( brush.vertices().count() < 1 ) dbg.nospace() << "None.\n";
    else
    {
        for ( int i = 0; i < brush.vertices().count(); i++ )
        {
            dbg.nospace() << brush.vertices().at(i)->handle() << " " << brush.vertices().at(i) << "\n";
        }
    }
    
    dbg.nospace() << "Edges:\n";
    if ( brush.edges().count() < 1 ) dbg.nospace() << "None.\n";
    else
    {
        for ( int i = 0; i < brush.edges().count(); i++ )
        {
            dbg.nospace() << brush.edges().at(i)->handle() << " " << brush.edges().at(i) << "\n";
        }
    }
    
    dbg.nospace() << "Faces:\n";
    if ( brush.faces().count() < 1 ) dbg.nospace() << "None.\n";
    else
    {
        for ( int i = 0; i < brush.faces().count(); i++ )
        {
            dbg.nospace() << brush.faces().at(i)->handle() << " " << brush.faces().at(i) << "\n";
        }
    }
    
    return dbg.space();
}

MODEL_BEGIN_NAMESPACE

MapBrush::MapBrush(MapDocument* document, QObject *parent) :
    MapComponent(parent), m_VertexList(VERTICES_TITLE, this),
    m_EdgeList(EDGES_TITLE, this), m_FaceList(FACES_TITLE, this),
    m_pHierarchyNode(new HierarchyNode3D(this)), m_pDocument(document)
{
    // Changing the vertices etc. should send the signal that our brush data is being modified.
    // When these slots are called, they also hook up their elements so that if an element
    // property changes, brushRenderDataChanged() is emitted.
    connect(&m_VertexList.signalRelay, &ObjectListSignals::itemAddedMapVertex,
            this, &MapBrush::handleVertexAdded);
    connect(&m_VertexList.signalRelay, &ObjectListSignals::itemRemovedMapVertex,
            this, &MapBrush::handleVertexRemoved);

    connect(&m_EdgeList.signalRelay, &ObjectListSignals::itemAddedMapEdge,
            this, &MapBrush::handleEdgeAdded);
    connect(&m_EdgeList.signalRelay, &ObjectListSignals::itemRemovedMapEdge,
            this, &MapBrush::handleEdgeRemoved);

    connect(&m_FaceList.signalRelay, &ObjectListSignals::itemAddedMapFace,
            this, &MapBrush::handleFaceAdded);
    connect(&m_FaceList.signalRelay, &ObjectListSignals::itemRemovedMapFace,
            this, &MapBrush::handleFaceRemoved);

    // Handle when lists are cleared.
    connect(&m_VertexList.signalRelay, SIGNAL(aboutToClear()),
            this, SLOT(handleClearVertices()));
    connect(&m_VertexList.signalRelay, SIGNAL(cleared()),
            this, SIGNAL(brushRenderDataChanged()));

    connect(&m_EdgeList.signalRelay, SIGNAL(aboutToClear()),
            this, SLOT(handleClearEdges()));
    connect(&m_EdgeList.signalRelay, SIGNAL(cleared()),
            this, SIGNAL(brushRenderDataChanged()));

    connect(&m_FaceList.signalRelay, SIGNAL(aboutToClear()),
            this, SLOT(handleClearFaces()));
    connect(&m_FaceList.signalRelay, SIGNAL(cleared()),
            this, SIGNAL(brushRenderDataChanged()));
    
    // Handle origin changes.
    connect(&hierarchyNode(), SIGNAL(originAboutToChange(QVector3D)),
            this, SLOT(fixUpVerticesForNewOrigin(QVector3D)));
    
    // Handle any kinds of hierarchy change.
    connect(&hierarchyNode(), SIGNAL(rotationChanged(QVector3D)), this, SLOT(handleVectorTransformationChanged(QVector3D)));
    connect(&hierarchyNode(), SIGNAL(translationChanged(QVector3D)), this, SLOT(handleVectorTransformationChanged(QVector3D)));
    connect(&hierarchyNode(), SIGNAL(scaleChanged(QVector3D)), this, SLOT(handleVectorTransformationChanged(QVector3D)));
}

void MapBrush::handleVectorTransformationChanged(const QVector3D &newVec)
{
    Q_UNUSED(newVec);
    emit brushRenderDataChanged();
}

HierarchyNode3D& MapBrush::hierarchyNode()
{
    return *m_pHierarchyNode;
}

const HierarchyNode3D& MapBrush::hierarchyNode() const
{
    return *m_pHierarchyNode;
}

void MapBrush::fixUpVerticesForNewOrigin(const QVector3D &origin)
{
    // Modify all the vertices to account for this new origin.
    QVector3D translation = hierarchyNode().origin() - origin;

    for ( int i = 0; i < vertices().count(); i++ )
    {
        MapVertex* v = vertices().at(i);
        v->setPosition(v->position() + translation);
    }
}

void MapBrush::handleVertexAdded(MapVertex *v)
{
    connect(v, SIGNAL(vertexRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    v->setDocument(document());
    sendBrushChanged();
}

void MapBrush::handleVertexRemoved(MapVertex *v)
{
    disconnect(v, SIGNAL(vertexRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    v->setDocument(NULL);
    sendBrushChanged();
}

void MapBrush::handleFaceAdded(MapFace *f)
{
    connect(f, SIGNAL(faceRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    f->setDocument(document());
    sendBrushChanged();
}

void MapBrush::handleFaceRemoved(MapFace *f)
{
    disconnect(f, SIGNAL(faceRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    f->setDocument(NULL);
    sendBrushChanged();
}

void MapBrush::handleEdgeAdded(MapEdge *e)
{
    connect(e, SIGNAL(edgeRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    e->setDocument(document());
    sendBrushChanged();
}

void MapBrush::handleEdgeRemoved(MapEdge *e)
{
    disconnect(e, SIGNAL(edgeRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    e->setDocument(NULL);
    sendBrushChanged();
}

void MapBrush::handleClearVertices()
{
    // Disconnect from vertices.
    for ( int i = 0; i < m_VertexList.count(); i++ )
    {
        disconnect(m_VertexList.at(i), SIGNAL(vertexRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    }
}

void MapBrush::handleClearEdges()
{
    // Disconnect from edges.
    for ( int i = 0; i < m_EdgeList.count(); i++ )
    {
        disconnect(m_EdgeList.at(i), SIGNAL(edgeRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    }
}

void MapBrush::handleClearFaces()
{
    // Disconnect from vertices.
    for ( int i = 0; i < m_FaceList.count(); i++ )
    {
        disconnect(m_FaceList.at(i), SIGNAL(faceRenderDataChanged()), this, SIGNAL(brushRenderDataChanged()));
    }
}

MapBrush::~MapBrush()
{
    // Disconnect from all vertex signals, then disconnect from the aboutToClear() signal.
    // This ensures that we don't crash the program upon destruction.
    m_VertexList.clear();
    m_EdgeList.clear();
    m_FaceList.clear();
    
    disconnect(&hierarchyNode(), SIGNAL(rotationChanged(QVector3D)), this, SLOT(handleVectorTransformationChanged(QVector3D)));
    disconnect(&hierarchyNode(), SIGNAL(translationChanged(QVector3D)), this, SLOT(handleVectorTransformationChanged(QVector3D)));
    disconnect(&hierarchyNode(), SIGNAL(scaleChanged(QVector3D)), this, SLOT(handleVectorTransformationChanged(QVector3D)));
    disconnect(&m_VertexList.signalRelay, &ObjectListSignals::itemAddedMapVertex,
            this, &MapBrush::handleVertexAdded);
    disconnect(&m_VertexList.signalRelay, &ObjectListSignals::itemRemovedMapVertex,
            this, &MapBrush::handleVertexRemoved);
    disconnect(&m_EdgeList.signalRelay, &ObjectListSignals::itemAddedMapEdge,
            this, &MapBrush::handleEdgeAdded);
    disconnect(&m_EdgeList.signalRelay, &ObjectListSignals::itemRemovedMapEdge,
            this, &MapBrush::handleEdgeRemoved);
    disconnect(&m_FaceList.signalRelay, &ObjectListSignals::itemAddedMapFace,
            this, &MapBrush::handleFaceAdded);
    disconnect(&m_FaceList.signalRelay, &ObjectListSignals::itemRemovedMapFace,
            this, &MapBrush::handleFaceRemoved);
    disconnect(&m_VertexList.signalRelay, SIGNAL(cleared()),
            this, SIGNAL(brushRenderDataChanged()));
    disconnect(&m_EdgeList.signalRelay, SIGNAL(cleared()),
            this, SIGNAL(brushRenderDataChanged()));
    disconnect(&m_FaceList.signalRelay, SIGNAL(cleared()),
            this, SIGNAL(brushRenderDataChanged()));
}

MapBrush::MapVertexList& MapBrush::vertices()
{
    return m_VertexList;
}

const MapBrush::MapVertexList& MapBrush::vertices() const
{
    return m_VertexList;
}

MapBrush::MapEdgeList& MapBrush::edges()
{
    return m_EdgeList;
}

const MapBrush::MapEdgeList& MapBrush::edges() const
{
    return m_EdgeList;
}

MapBrush::MapFaceList& MapBrush::faces()
{
    return m_FaceList;
}

const MapBrush::MapFaceList& MapBrush::faces() const
{
    return m_FaceList;
}

MapDocument* MapBrush::document()
{
    return m_pDocument;
}

const MapDocument* MapBrush::document() const
{
    return m_pDocument;
}

void MapBrush::setDocument(MapDocument *doc)
{
    if ( m_pDocument == doc ) return;

    m_pDocument = doc;

    for ( int i = 0; i < m_VertexList.count(); i++ )
    {
        m_VertexList.at(i)->setDocument(doc);
    }

    for ( int i = 0; i < m_FaceList.count(); i++ )
    {
        m_FaceList.at(i)->setDocument(doc);
    }

    for ( int i = 0; i < m_EdgeList.count(); i++ )
    {
        m_EdgeList.at(i)->setDocument(doc);
    }
}

void MapBrush::removedFromDocument(MapDocument *doc)
{
    setDocument(NULL);

    // TODO: Fill this in when we have stuff to clean up.
    Q_UNUSED(doc);
}

void MapBrush::handleSelectionChanged(bool selected)
{
    // Change the colour of our vertices.
    // We'll probably want to do this in a better way later on.
    for ( int i = 0; i < vertices().count(); i++ )
    {
        vertices().at(i)->setColor(selected ? QColor(Qt::red) : QColor(Qt::white));
    }
}

MODEL_END_NAMESPACE
