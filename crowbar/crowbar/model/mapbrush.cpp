#include "mapbrush.h"
#include <QHash>

MODEL_BEGIN_NAMESPACE

void MapBrush::initialise(MapComponent *parent)
{
    // WHOOPS. Turns out if we just pass the parent down to MapComponent
    // then document()->notifyComponentAdded() only gets a MapComponent
    // object because the MapBrush is not at that point fully constructed.
    // Therefore, we pass NULL and set the parent manually here.
    setParentComponent(parent);

    m_pVertices = new VertexVector(this);
    m_pEdges = new EdgeVector(this);
    m_pFaces = new FaceVector(this);

    connectSignals();
}

void MapBrush::dispose()
{
    // Ensure that all our lists are cleared before anything else happens.
    m_pFaces->deleteAll();
    m_pEdges->deleteAll();
    m_pVertices->deleteAll();
}

void MapBrush::connectSignals()
{
    connect(m_pVertices, SIGNAL(itemAdded(MapVertex*)), this, SLOT(handleVertexAdded(MapVertex*)));
    connect(m_pVertices, SIGNAL(itemRemoved(MapVertex*)), this, SLOT(handleVertexRemoved(MapVertex*)));

    connect(m_pEdges, SIGNAL(itemAdded(MapEdge*)), this, SLOT(handleEdgeAdded(MapEdge*)));
    connect(m_pEdges, SIGNAL(itemRemoved(MapEdge*)), this, SLOT(handleEdgeRemoved(MapEdge*)));

    connect(m_pFaces, SIGNAL(itemAdded(MapFace*)), this, SLOT(handleFaceAdded(MapFace*)));
    connect(m_pFaces, SIGNAL(itemRemoved(MapFace*)), this, SLOT(handleFaceRemoved(MapFace*)));
}

MapBrush::VertexVector& MapBrush::vertices()
{
    return *m_pVertices;
}

const MapBrush::VertexVector& MapBrush::vertices() const
{
    return *m_pVertices;
}

MapVertex* MapBrush::vertices(int index)
{
    return m_pVertices->constData().at(index);
}

const MapVertex* MapBrush::vertices(int index) const
{
    return m_pVertices->constData().at(index);
}

MapBrush::EdgeVector& MapBrush::edges()
{
    return *m_pEdges;
}

const MapBrush::EdgeVector& MapBrush::edges() const
{
    return *m_pEdges;
}

MapEdge* MapBrush::edges(int index)
{
    return m_pEdges->constData().at(index);
}

const MapEdge* MapBrush::edges(int index) const
{
    return m_pEdges->constData().at(index);
}

MapBrush::FaceVector& MapBrush::faces()
{
    return *m_pFaces;
}

const MapBrush::FaceVector& MapBrush::faces() const
{
    return *m_pFaces;
}

MapFace* MapBrush::faces(int index)
{
    return m_pFaces->constData().at(index);
}

const MapFace* MapBrush::faces(int index) const
{
    return m_pFaces->constData().at(index);
}

MapVertex* MapBrush::newVertex(const QVector3D &pos, const QColor &col)
{
    MapVertex* v = new MapVertex(pos, col, this);
    vertices().append(v);
    return v;
}

MapEdge* MapBrush::newEdge(MapVertex *v0, MapVertex *v1)
{
    MapEdge* e = new MapEdge(v0, v1, this);
    edges().append(e);
    return e;
}

MapFace* MapBrush::newFace()
{
    MapFace* f = new MapFace(this);
    faces().append(f);
    return f;
}

void MapBrush::handleVertexAdded(MapVertex *v)
{
    if ( !v ) return;

    connect(v, SIGNAL(destroyed(QObject*)), this, SLOT(detachVertex(QObject*)));
}

void MapBrush::handleVertexRemoved(MapVertex *v)
{
    if ( !v ) return;
    v->setParentComponent(NULL);
    disconnect(v, SIGNAL(destroyed(QObject*)), this, SLOT(detachVertex(QObject*)));
}

void MapBrush::handleEdgeAdded(MapEdge *e)
{
    if ( !e ) return;

    connect(e, SIGNAL(destroyed(QObject*)), this, SLOT(detachEdge(QObject*)));
}

void MapBrush::handleEdgeRemoved(MapEdge *e)
{
    if ( !e ) return;
    e->setParentComponent(NULL);
    disconnect(e, SIGNAL(destroyed(QObject*)), this, SLOT(detachEdge(QObject*)));
}

void MapBrush::handleFaceAdded(MapFace *f)
{
    if ( !f ) return;

    connect(f, SIGNAL(destroyed(QObject*)), this, SLOT(detachFace(QObject*)));
    connect(f, SIGNAL(textureInfoChanged()), this, SLOT(sendRenderDataChange()));
}

void MapBrush::handleFaceRemoved(MapFace *f)
{
    if ( !f ) return;
    f->setParentComponent(NULL);
    disconnect(f, SIGNAL(destroyed(QObject*)), this, SLOT(detachFace(QObject*)));
    disconnect(f, SIGNAL(textureInfoChanged()), this, SLOT(sendRenderDataChange()));
}

void MapBrush::deleteAll()
{
    faces().deleteAll();
    edges().deleteAll();
    vertices().deleteAll();
}

void MapBrush::detachVertex(QObject *v)
{
    MapVertex* vertex = qobject_cast<MapVertex*>(v);
    Q_ASSERT(vertex);

    vertices().removeAt(vertices().constData().indexOf(vertex));
}

void MapBrush::detachEdge(QObject *e)
{
    MapEdge* edge = qobject_cast<MapEdge*>(e);
    Q_ASSERT(edge);

    edges().removeAt(edges().constData().indexOf(edge));
}

void MapBrush::detachFace(QObject *f)
{
    MapFace* face = qobject_cast<MapFace*>(f);
    Q_ASSERT(face);

    faces().removeAt(faces().constData().indexOf(face));
}

MapComponent* MapBrush::clone() const
{
    MapBrush* brush = new MapBrush();
    cloneProperties(brush);
    return brush;
}

void MapBrush::cloneProperties(MapComponent *c) const
{
    MapBrush* brush = qobject_cast<MapBrush*>(c);
    if ( !brush ) return;
    
    // Keep the same parent.
    brush->setParentComponent(parentComponent());
    
    brush->hierarchyTransform()->setRotation(hierarchyTransform()->rotation());
    brush->hierarchyTransform()->setTranslation(hierarchyTransform()->translation());
    brush->hierarchyTransform()->setScale(hierarchyTransform()->scale());
    brush->hierarchyTransform()->setTemporaryRotation(hierarchyTransform()->temporaryRotation());
    brush->hierarchyTransform()->setTemporaryTranslation(hierarchyTransform()->temporaryTranslation());
    brush->hierarchyTransform()->setTemporaryScale(hierarchyTransform()->temporaryScale());
    
    // TODO: Give sub-components a clone() function rather than doing this all manually.
    
    // Clone vertices first, keeping a hash table of old->new vertices.
    QHash<const MapVertex*, MapVertex*> vHash;
    
    for ( int i = 0; i < vertices().constData().count(); i++ )
    {
        const MapVertex* vOld = vertices(i);
        MapVertex* vNew = brush->newVertex(vOld->position(), vOld->color());
        vHash.insert(vOld, vNew);
    }
    
    // Clone edges by using new vertices.
    for ( int i = 0; i < edges().constData().count(); i++ )
    {
        const MapEdge* eOld = edges(i);
        brush->newEdge(vHash.value(eOld->vertex0()), vHash.value(eOld->vertex1()));
    }
    
    // Clone faces by using new vertices.
    for ( int i = 0; i < faces().constData().count(); i++ )
    {
        const MapFace* fOld = faces(i);
        MapFace* fNew = brush->newFace();
        
        for ( int j = 0; j < fOld->vertices().constData().count(); j++ )
        {
            fNew->vertices().append(vHash.value(fOld->vertices(j)));
        }
        
        fNew->textureInfo()->setAlignToFace(fOld->textureInfo()->alignToFace());
        fNew->textureInfo()->setAxisSpace(fOld->textureInfo()->axisSpace());
        fNew->textureInfo()->setLightmapScale(fOld->textureInfo()->lightmapScale());
        fNew->textureInfo()->setRotation(fOld->textureInfo()->rotation());
        fNew->textureInfo()->setScale(fOld->textureInfo()->scale());
        fNew->textureInfo()->setSmoothingGroups(fOld->textureInfo()->smoothingGroups());
        fNew->textureInfo()->setTexture(fOld->textureInfo()->texture());
        fNew->textureInfo()->setTextureUAxis(fOld->textureInfo()->textureUAxis());
        fNew->textureInfo()->setTextureVAxis(fOld->textureInfo()->textureVAxis());
        fNew->textureInfo()->setTranslation(fOld->textureInfo()->translation());
    }
}

void MapBrush::setColor(const QColor &col)
{
    if ( col == QColor() ) return;
    
    for ( int i = 0; i < vertices().constData().count(); i++ )
    {
        vertices(i)->setColor(col);
    }
}

QBox3D MapBrush::localBoundingBox() const
{
    if ( vertices().constData().count() < 1 ) return QBox3D();

    // Keep a track of the lowest and highest values we've found.
    QVector3D min = vertices(0)->position();
    QVector3D max = min;

    for ( int i = 1; i < vertices().constData().count(); i++ )
    {
        QVector3D pos = vertices(i)->position();

        if ( pos.x() < min.x() ) min.setX(pos.x());
        else if ( pos.x() > max.x() ) max.setX(pos.x());

        if ( pos.y() < min.y() ) min.setY(pos.y());
        else if ( pos.y() > max.y() ) max.setY(pos.y());

        if ( pos.z() < min.z() ) min.setZ(pos.z());
        else if ( pos.z() > max.z() ) max.setZ(pos.z());
    }

    return QBox3D(min, max);
}

QVector3D MapBrush::localCentroid() const
{
    if ( vertices().constData().count() < 1 ) return QVector3D();

    return localBoundingBox().center();
}

void outputPrecisionInfo(const MapBrush &brush)
{
    qDebug() << "Brush" << brush.objectName() << "global bounding box:" << brush.globalBoundingBox()
             << "Local bounding box:" << brush.localBoundingBox()
             << "World position:" << brush.hierarchyTransform()->overallTranslation();
}

void MapBrush::setAllVerticesSelected(bool selected)
{
    for ( int i = 0; i < vertices().count(); i++ )
    {
        vertices(i)->setSelected(selected);
    }
}

void MapBrush::setAllEdgesSelected(bool selected)
{
    for ( int i = 0; i < edges().count(); i++ )
    {
        edges(i)->setSelected(selected);
    }
}

void MapBrush::setAllFacesSelected(bool selected)
{
    for ( int i = 0; i < faces().count(); i++ )
    {
        faces(i)->setSelected(selected);
    }
}

void MapBrush::setAllSubComponentsSelected(bool selected)
{
    setAllVerticesSelected(selected);
    setAllEdgesSelected(selected);
    setAllFacesSelected(selected);
}

void MapBrush::translateOriginInPlace(const QVector3D &translation)
{
    // Translating the origin by T means translating all the vertices by -T.
    hierarchyTransform()->setTranslation(hierarchyTransform()->translation() + translation);
    translateVertices(-translation);
}

void MapBrush::setOriginInPlace(const QVector3D &position)
{
    // Set the position by translating to it.
    translateOriginInPlace(position - hierarchyTransform()->translation());
}

void MapBrush::translateVertices(const QVector3D &translation)
{
    for ( int i = 0; i < vertices().count(); i++ )
    {
        MapVertex* v = vertices(i);
        v->setPosition(v->position() + translation);
    }
}

MODEL_END_NAMESPACE
