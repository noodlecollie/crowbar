#include "mapface.h"
#include "mapvertex.h"

MODEL_BEGIN_NAMESPACE

MapFace::MapFace(MapComponent *parent) :
    PositionalSubComponent(parent)
{
    m_pVertices = new VertexVector(this);
    m_pTextureInfo = new FaceTextureInfo(this);
    m_bUpdateTextureAxes = false;
    m_bShouldUpdateTextureAxes = false;
    setupConnections();
}

void MapFace::setupConnections()
{
    connect(m_pVertices, SIGNAL(itemAdded(MapVertex*)), this, SLOT(handleItemAdded(MapVertex*)));
    connect(m_pVertices, SIGNAL(itemRemoved(MapVertex*)), this, SLOT(handleItemRemoved(MapVertex*)));
    connect(m_pTextureInfo, SIGNAL(dataChanged()), this, SIGNAL(textureInfoChanged()));
}

MapFace::VertexVector& MapFace::vertices()
{
    return *m_pVertices;
}

const MapFace::VertexVector& MapFace::vertices() const
{
    return *m_pVertices;
}

bool MapFace::isValid() const
{
    return vertices().constData().count() > 2;
}

QVector3D MapFace::normal() const
{
    if ( !isValid() ) return QVector3D();

    return QVector3D::normal(vertices(0)->position(),
                             vertices(1)->position(),
                             vertices(2)->position());
}

QVector3D MapFace::globalNormal() const
{
    if ( !isValid() ) return QVector3D();
    
    QMatrix4x4 mat = parentComponent()->hierarchyTransform()->globalTransformMatrix().inverted();
    return mat * normal();
}

MapVertex* MapFace::vertices(int index)
{
    return vertices().constData().at(index);
}

const MapVertex* MapFace::vertices(int index) const
{
    return vertices().constData().at(index);
}

void MapFace::connectChangeSignal(MapVertex *vertex)
{
    connect(vertex, SIGNAL(renderDataChanged(ChangeId_t)), this, SIGNAL(renderDataChanged(ChangeId_t)));
    connect(vertex, SIGNAL(renderDataChanged(ChangeId_t)), this, SLOT(vertexUpdated()));
    connect(vertex, SIGNAL(destroyed(QObject*)), this, SLOT(detachVertex(QObject*)));
}

void MapFace::disconnectChangeSignal(MapVertex *vertex)
{
    disconnect(vertex, SIGNAL(renderDataChanged(ChangeId_t)), this, SIGNAL(renderDataChanged(ChangeId_t)));
    disconnect(vertex, SIGNAL(renderDataChanged(ChangeId_t)), this, SLOT(vertexUpdated()));
    disconnect(vertex, SIGNAL(destroyed(QObject*)), this, SLOT(detachVertex(QObject*)));
}

void MapFace::handleItemAdded(MapVertex *vertex)
{
    connectChangeSignal(vertex);
    emit vertexAdded(vertex);
    sendRenderDataChange();
}

void MapFace::handleItemRemoved(MapVertex *vertex)
{
    disconnectChangeSignal(vertex);
    emit vertexRemoved(vertex);
    sendRenderDataChange();
}

QBox3D MapFace::boundingBox() const
{
    if ( !isValid() ) return QBox3D();

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

QVector3D MapFace::centroid() const
{
    if ( !isValid() ) return QVector3D();

    // Sum all the vertex positions and divide by the number of vertices.
    QVector3D sum;

    for ( int i = 0; i < vertices().constData().count(); i++ )
    {
        sum += vertices(i)->position();
    }

    sum /= vertices().constData().count();

    return sum;
}

void MapFace::detachVertex(QObject *v)
{
    MapVertex* vertex = qobject_cast<MapVertex*>(v);
    Q_ASSERT(vertex);

    vertices().removeAt(vertices().constData().indexOf(vertex));
}

bool MapFace::autoUpdateTextureAxes() const
{
    return m_bUpdateTextureAxes;
}

void MapFace::setAutoUpdateTextureAxes(bool enabled)
{
    if ( enabled == m_bUpdateTextureAxes ) return;
    
    m_bUpdateTextureAxes = enabled;
    m_bShouldUpdateTextureAxes = true;
    emit autoUpdateTextureAxesChanged(m_bUpdateTextureAxes);
}

void MapFace::vertexUpdated()
{
    if ( m_bUpdateTextureAxes ) m_bShouldUpdateTextureAxes = true;
}

void MapFace::generateTextureAxesIfRequired()
{
    if ( m_bUpdateTextureAxes ) autoGenerateTextureAxes();
}

FaceTextureInfo* MapFace::textureInfo()
{
    if ( m_bUpdateTextureAxes && m_bShouldUpdateTextureAxes )
    {
        m_pTextureInfo->autoGenerateTextureAxes(normal());
        m_bShouldUpdateTextureAxes = false;
    }
    
    return m_pTextureInfo;
}

const FaceTextureInfo* MapFace::textureInfo() const
{
    if ( m_bUpdateTextureAxes && m_bShouldUpdateTextureAxes )
    {
        m_pTextureInfo->autoGenerateTextureAxes(normal());
        m_bShouldUpdateTextureAxes = false;
    }

    return m_pTextureInfo;
}

void MapFace::autoGenerateTextureAxes()
{
    textureInfo()->autoGenerateTextureAxes(textureInfo()->axisSpace() == FaceTextureInfo::WorldSpace ? globalNormal() : normal());
}

QPlane3D MapFace::toPlane3D() const
{
    if ( !isValid() ) return QPlane3D();

    return QPlane3D(centroid(), normal());
}

void MapFace::flip()
{
    int count = vertices().count();
    for ( int i = 0; i < count/2; i++ )
    {
        MapVertex* temp = vertices(count-i-1);
        vertices().removeAt(count-i-1);
        vertices().insert(count-i-1, vertices(i));
        vertices().removeAt(i);
        vertices().insert(i, temp);
    }
}

MODEL_END_NAMESPACE
