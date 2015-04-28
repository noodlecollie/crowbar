#include "renderentrymapbrush.h"
#include "view_util.h"
#include "mapbrush.h"
#include "mapedge.h"
#include "mapvertex.h"
#include <QGLBuilder>
#include <QGLPainter>
#include "mapdocument.h"
#include <QGLMaterial>
#include <QGLMaterialCollection>
#include <QSharedPointer>
#include <QRay3D>
#include <QGLTwoSidedMaterial>
#include "vieweffects.h"

#define PLANE_TEXTURE "/texture_plane"

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

RenderEntryMapBrush::RenderEntryMapBrush(MapBrush *component, MapDocumentView* parent) :
    RenderTableEntry(component, parent)
{
}

RenderEntryMapBrush::~RenderEntryMapBrush()
{
}

// If col == QColor(), vertex colour is used.
QGeometryData RenderEntryMapBrush::faceData(const MapFace *face, const QColor &col, bool flipFaces)
{
    Q_ASSERT(face);
    Q_ASSERT(face->parentComponent());
    bool vCol = (col == QColor());
    if ( face->vertices().count() < 3 ) return QGeometryData();

    QGeometryData g;
    for ( int i = 0; i < face->vertices().constData().count(); i++ )
    {
        const MapVertex* vertex = face->vertices(i);
        g.appendVertex(vertex->position());
        g.appendColor(vCol ? vertex->color() : col);
        g.appendNormal(face->normal());
    }

    for ( int i = 2; i < face->vertices().constData().count(); i++ )
    {
        int n1 = flipFaces ? i : i-1;
        int n2 = flipFaces ? i-1 : i;

        g.appendIndex(0);
        g.appendIndex(n1);
        g.appendIndex(n2);
    }
    
    addTextureCoordinates(face, g);

    return g;
}

void RenderEntryMapBrush::addTextureCoordinates(const MapFace *face, QGeometryData &data)
{
    Q_ASSERT(face);
    
    // First of all, find out what co-ordinate space the texture plane is in.
    QList<QVector3D> vertices;
    switch ( face->textureInfo()->axisSpace() )
    {
        case FaceTextureInfo::ParentSpace:
        {
            // We need to transform the vertices into parent space first.
            QMatrix4x4 mat = face->parentComponent()->hierarchyTransform()->transformMatrix().inverted();
            for ( int i = 0; i < face->vertices().constData().count(); i++ )
            {
                vertices.append(mat * face->vertices(i)->position());
            }
            break;
        }
        
        case FaceTextureInfo::WorldSpace:
        {
            // We need to transform the vertices into world space first.
            QMatrix4x4 mat = face->parentComponent()->hierarchyTransform()->globalTransformMatrix();
            for ( int i = 0; i < face->vertices().constData().count(); i++ )
            {
                vertices.append(mat * face->vertices(i)->position());
            }
            break;
        }
        
        default:
        {
            // The vertices are already in the correct space.
            for ( int i = 0; i < face->vertices().constData().count(); i++ )
            {
                vertices.append(face->vertices(i)->position());
            }
            break;
        }
    }
    
    QList<QVector2D> coordinates = View_Util::textureCoordinates(*face->textureInfo(), vertices);
    foreach ( QVector2D vec, coordinates )
    {
        data.appendTexCoord(vec);
    }
}

QGLSceneNode* RenderEntryMapBrush::sceneNodeFor(IRender::RenderMode mode) const
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

QGeometryData RenderEntryMapBrush::edges(const MapBrush *brush, const QColor &col)
{
    QGeometryData data;
    int index = 0;

    for ( int i = 0; i < brush->edges().constData().count(); i++ )
    {
        const MapEdge* edge = brush->edges(i);
        const MapVertex* v0 = edge->vertex0();
        const MapVertex* v1 = edge->vertex1();

        data.appendVertex(v0->position());
        data.appendColor(col);
        data.appendIndex(index);
        index++;

        data.appendVertex(v1->position());
        data.appendColor(col);
        data.appendIndex(index);
        index++;
    }

    return data;
}

void RenderEntryMapBrush::handleFaceTexture(const MapFace *face, QSharedPointer<QGLMaterialCollection> collection, QGLSceneNode *node, const QColor &col)
{
    Q_ASSERT(face);
    Q_ASSERT(node);
    
    // Get the texture associated with the face.
    QGLTexture2D* tex = face->textureInfo()->texture();
    Q_ASSERT(tex);
    
    if ( collection->contains(tex->objectName()) )
    {
        node->setMaterial(collection->material(tex->objectName()));
        return;
    }
    
    QGLMaterial* mat = new QGLMaterial();
    
    // Until we can get the colour blending shader to work... >:C
    mat->setAmbientColor(col);
    
    mat->setDiffuseColor(QColor(0,0,0));
    mat->setSpecularColor(QColor(0,0,0));
    mat->setTexture(tex);
    mat->setTextureCombineMode(QGLMaterial::Modulate);
    collection->addMaterial(mat);
    node->setPalette(collection);
    node->setMaterial(mat);
}

QGLSceneNode* RenderEntryMapBrush::buildSolidNode() const
{
    MapBrush* br = brush()->isProxy() ? brush()->proxyTargetBrush() : brush();
    QColor renderColour(255,255,255);
    bool selected = false;

    // If an odd number of axes are negative, we need to flip faces.
    bool flipFaces = false;
    QVector3D scale = br->hierarchyTransform()->overallScale();
    if ( scale.x() < 0.0f ) flipFaces = !flipFaces;
    if ( scale.y() < 0.0f ) flipFaces = !flipFaces;
    if ( scale.z() < 0.0f ) flipFaces = !flipFaces;

    if ( brush()->isProxy() )
    {
        QVector3D scale = brush()->hierarchyTransform()->overallScale();
        if ( scale.x() < 0.0f ) flipFaces = !flipFaces;
        if ( scale.y() < 0.0f ) flipFaces = !flipFaces;
        if ( scale.z() < 0.0f ) flipFaces = !flipFaces;
    }
    
    // Proxy
    if ( brush()->isProxy() )
    {
        if ( brush()->proxyTargetBrush()->selected() || brush()->selected() )
        {
            renderColour = QColor(0,101,145);
            selected = true;
        }
    }

    // No proxy
    else
    {
        if ( brush()->selected() )
        {
            renderColour = QColor(255,0,0);
            selected = true;
        }

        // This'll probably be kind of slow...
        else
        {
            QList<MapComponent*> list = brush()->childComponents();
            foreach ( MapComponent* c, list )
            {
                if ( brush()->document()->selectedSet().contains(c) )
                {
                    renderColour = QColor(101,205,241);
                    break;
                }
            }
        }
    }

    QGLSceneNode* builderNode = new QGLSceneNode();
    QSharedPointer<QGLMaterialCollection> collection(new QGLMaterialCollection());
    builderNode->setPalette(collection);
    QGLSceneNode* texturePlanes = NULL;

    for ( int i = 0; i < br->faces().constData().count(); i++ )
    {
        MapFace* f = br->faces(i);
        QGeometryData data = faceData(f, renderColour, flipFaces);

        QGLSceneNode* n = new QGLSceneNode;
        n->setEffect(QGL::LitModulateTexture2D);
        //n->setUserEffect(Effects::presetEffect(Effects::ColourBlendedTexture));   // WHY WON'T YOU WORK

        n->setGeometry(data);
        n->setStart(0);
        n->setCount(data.indexCount());
        n->setDrawingMode(QGL::TriangleFan);
        
        QColor faceColour = f->selected() ? QColor(255,94,129) : renderColour;
        
        handleFaceTexture(f, collection, n, faceColour);
        
        builderNode->addNode(n);
//        if ( f->selected() )
//        {
//            if ( !texturePlanes ) texturePlanes = new QGLSceneNode();
//            texturePlanes->addNode(buildTexturePlaneNode(f, collection));
//        }
    }

    QGLSceneNode* wrapper = new QGLSceneNode();
    wrapper->addNode(builderNode);

    if ( selected )
    {
        QGLSceneNode* origin = View_Util::solidOriginMarker(32.0f);
        wrapper->addNode(origin);
    }

    if ( texturePlanes )
    {
        wrapper->addNode(texturePlanes);
    }

    if ( brush()->hidden() )
    {
        wrapper->setOptions(builderNode->options() | QGLSceneNode::HideNode);
    }

    return wrapper;
}

QGLSceneNode* RenderEntryMapBrush::buildWireframeNode() const
{
    MapBrush* br = brush()->isProxy() ? brush()->proxyTargetBrush() : brush();
    QColor renderColour(255,255,255);

    // Proxy
    if ( brush()->isProxy() )
    {
        if ( brush()->proxyTargetBrush()->selected() || brush()->selected() )
        {
            renderColour = QColor(18,183,255);
        }
    }

    // No proxy
    else
    {
        if ( brush()->selected() )
        {
            renderColour = QColor(255,0,0);
        }

        else
        {
            QList<MapComponent*> list = brush()->childComponents();
            foreach ( MapComponent* c, list )
            {
                if ( brush()->document()->selectedSet().contains(c) )
                {
                    renderColour = QColor(101,205,241);
                    break;
                }
            }
        }
    }
    
    // Get the wireframe geometry. This is drawn with lines.
    QGLSceneNode* node = View_Util::sceneNodeForWireframe(edges(br, renderColour));

    // Get the centre point. This is drawn just as a point.
    if ( node->geometry().count() < 1 )
    {
        qWarning() << "RenderEntryMapBrush::BuildWireframeNode: Brush" << br
                   << "with parent" << (br->parentComponent() ? br->parentComponent()->metaObject()->className()
                                                                   : "NULL")
                   << "has no geometry.";
    }

    QGLSceneNode* centre = wireframeCentreMarker(QVector3D(0,0,0), EDGE_CENTROID_SIZE, renderColour);    // TODO: Change width here.

    // Wrap them both in a container node.
    QGLSceneNode* wrapper = new QGLSceneNode();
    wrapper->addNode(node);
    wrapper->addNode(centre);
    wrapper->setEffect(QGL::FlatPerVertexColor);
    if ( brush()->hidden() )
    {
        wrapper->setOptions(wrapper->options() | QGLSceneNode::HideNode);
    }

    return wrapper;
}

void RenderEntryMapBrush::drawWireframe(QGLPainter *painter, const QSize &size, bool forSelection)
{
    Q_UNUSED(forSelection);
    
    QGLSceneNode* node = sceneNode(IRender::Edges);
    if ( !node ) return;

    // We need to decide whether to render the central point.
    // It seems to be the projection matrix that changes when we zoom in/out.
    // We need to multiply the bounding box of the geometry by the projection
    // matrix and see whether it's too small in comparison with the centre point.

    QList<QGLSceneNode*> list = node->children();
    QGLSceneNode* geometry = list.at(0);            // See buildWireframeNode().
    QGLSceneNode* centre = list.at(1);
    Q_ASSERT(geometry);
    Q_ASSERT(centre);

//    float lineWidth = geometry->drawingWidth();
//    float pointSize = centre->drawingWidth();

    Q_ASSERT(geometry && centre);

    QBox3D bbox = geometry->boundingBox();

    // If the bounding box is null, just draw the thing and forget it.
    if ( bbox.isNull() )
    {
//        if ( forSelection )
//        {
//            geometry->setDrawingWidth(lineWidth < LINE_SELECTION_BLOAT ? LINE_SELECTION_BLOAT : lineWidth);
//            centre->setDrawingWidth(pointSize < POINT_SELECTION_BLOAT ? POINT_SELECTION_BLOAT : pointSize);

//            node->draw(painter);

//            geometry->setDrawingWidth(lineWidth);
//            centre->setDrawingWidth(pointSize);

//            return;
//        }

        node->draw(painter);
        return;
    }

    // Because of the transformation, we will only need to be concerned with x and y.
    QVector3D max = painter->projectionMatrix().top() * bbox.maximum();
    QVector3D min = painter->projectionMatrix().top() * bbox.minimum();
    QVector2D bboxDelta(max.x() - min.x(), max.y() - min.y());

    // We'd like padding between the edge of the point and the edge of the bounding box.
    QVector2D padding((float)EDGE_CENTROID_PADDING_PIXELS/((float)size.width()/2.0f),
                      (float)EDGE_CENTROID_PADDING_PIXELS/((float)size.height()/2.0f));
    bboxDelta.setX(bboxDelta.x() - (2.0f * padding.x()));
    bboxDelta.setY(bboxDelta.y() - (2.0f * padding.y()));
    if ( bboxDelta.x() < 0.0f ) bboxDelta.setX(0.0f);
    if ( bboxDelta.y() < 0.0f ) bboxDelta.setY(0.0f);

    // To calculate the screen extent of the point, we can't just use the width as that's not in
    // screen-space co-ordinates.
    float pSize = centre->drawingWidth();
    QVector2D ssPoint(pSize / ((float)size.width()/2.0f), pSize / ((float)size.height()/2.0f));

//    if ( forSelection )
//    {
//        geometry->setDrawingWidth(/*lineWidth < LINE_SELECTION_BLOAT ? LINE_SELECTION_BLOAT : */lineWidth);
//        geometry->draw(painter);
//        geometry->setDrawingWidth(lineWidth);
//    }
//    else
//    {
        geometry->draw(painter);
//    }


    // Only draw the point if it fits inside the bounding box.
    if ( ssPoint.x() < bboxDelta.x() && ssPoint.y() < bboxDelta.y() )
    {
//        if ( forSelection )
//        {
//            centre->setDrawingWidth(pointSize < POINT_SELECTION_BLOAT ? POINT_SELECTION_BLOAT : pointSize);
//            centre->draw(painter);
//            centre->setDrawingWidth(pointSize);
//        }
//        else
//        {
            centre->draw(painter);
//        }
    }
}

bool RenderEntryMapBrush::drawDelegate(QGLPainter *painter, RenderMode mode, const QSize &viewSize, bool selection)
{
    // If we're drawing in wireframe then we handle this ourselves.
    // Otherwise we just let the default implementation draw.
    if ( mode == IRender::Edges )
    {
        drawWireframe(painter, viewSize, selection);
        return true;
    }
    
    return false;
}

QMatrix4x4 RenderEntryMapBrush::transformMatrix() const
{
    if ( brush()->isProxy() ) return brush()->hierarchyTransform()->transformMatrix()
            * brush()->proxyTarget()->hierarchyTransform()->transformMatrix();
    
    return component()->hierarchyTransform()->transformMatrix();
}

QGeometryData RenderEntryMapBrush::brushVerticesToGeometryData(MapBrush *brush)
{
    QGeometryData data;
    if ( !brush ) return data;

    for ( int i = 0; i < brush->vertices().constData().count(); i++ )
    {
        MapVertex* v = brush->vertices(i);
        data.appendVertex(v->position());
        data.appendColor(v->color());
        data.appendIndex(i);
    }

    return data;
}

QVector2D RenderEntryMapBrush::cheapTexCoordFor(const QVector3D &vertex, const QVector3D &faceNormal)
{
    // Use the co-ordinates of the plane that the face lies predominantly in.
    float dotX = qAbs<float>(QVector3D::dotProduct(faceNormal, QVector3D(1,0,0)));
    float dotY = qAbs<float>(QVector3D::dotProduct(faceNormal, QVector3D(0,1,0)));
    float dotZ = qAbs<float>(QVector3D::dotProduct(faceNormal, QVector3D(0,0,1)));

    // For now (as we're ignoring scale), make 128 world units cover the entire texture (ie. 0.0 - 1.0).
    float x, y;
    if ( dotX >= dotY && dotX >= dotZ )
    {
        x = vertex.y();
        y = vertex.z();
    }
    else if ( dotY >= dotX && dotY >= dotZ )
    {
        x = vertex.x();
        y = vertex.z();
    }
    else
    {
        x = vertex.x();
        y = vertex.y();
    }
    
    x /= 120.0f;
    y /= 128.0f;

    return QVector2D(x,y);
}

QGLSceneNode* RenderEntryMapBrush::buildTexturePlaneNode(const MapFace *face, QSharedPointer<QGLMaterialCollection> collection)
{
    Q_ASSERT(face);

    QVector3D normal = face->textureInfo()->texturePlaneNormal();
    QVector3D texU = face->textureInfo()->textureUAxis().normalized();
    QVector3D texV = face->textureInfo()->textureVAxis().normalized();
    if ( normal.isNull() ) return NULL;

    // Get the local bounds of the face.
    QBox3D bounds = face->boundingBox();

    // Expand the box slightly.
    QVector3D max = bounds.maximum();
    QVector3D min = bounds.minimum();
    QVector3D delta = max - min;
    float dmax = 0.1f * qMax<float>(delta.x(), qMax<float>(delta.y(), delta.z()));
    max = max + QVector3D(dmax, dmax, dmax);
    min = min - QVector3D(dmax, dmax, dmax);
    bounds.setExtents(max, min);
    QVector3D centre = bounds.center();

    // Generate the plane.
    QPlane3D plane = face->textureInfo()->toPlane3D();
    plane.setOrigin(centre);
    qDebug() << "Texture plane:" << plane;

    // Intersect the texture U axis with the box.
    float minIntersectionU = 0, maxIntersectionU = 0;
    QRay3D rayU(centre, texU);
    bool intersected = bounds.intersection(rayU, &minIntersectionU, &maxIntersectionU);
    Q_ASSERT(intersected);

    // Do the same for the V axis.
    float minIntersectionV = 0, maxIntersectionV = 0;
    QRay3D rayV(centre, texV);
    intersected = bounds.intersection(rayV, &minIntersectionV, &maxIntersectionV);
    Q_ASSERT(intersected);

    // Work out the corner vertices in terms of U and V.
    QVector2D minUminV(minIntersectionU, minIntersectionV);
    QVector2D minUmaxV(minIntersectionU, maxIntersectionV);
    QVector2D maxUminV(maxIntersectionU, minIntersectionV);
    QVector2D maxUmaxV(maxIntersectionU, maxIntersectionV);

    // Work out the corner vertices in 3D space.
    QVector3D minUminV3 = centre + (minUminV.x() * texU) + (minUminV.y() * texV);
    QVector3D minUmaxV3 = centre + (minUmaxV.x() * texU) + (minUmaxV.y() * texV);
    QVector3D maxUminV3 = centre + (maxUminV.x() * texU) + (maxUminV.y() * texV);
    QVector3D maxUmaxV3 = centre + (maxUmaxV.x() * texU) + (maxUmaxV.y() * texV);

    QGeometryData data;
    data.appendVertex(minUminV3, minUmaxV3, maxUmaxV3, maxUminV3);
    data.appendNormal(normal, normal, normal, normal);
    data.appendIndex(0);
    data.appendIndex(1);
    data.appendIndex(2);
    data.appendIndex(0);
    data.appendIndex(2);
    data.appendIndex(3);

    data.appendIndex(3);
    data.appendIndex(2);
    data.appendIndex(0);
    data.appendIndex(2);
    data.appendIndex(1);
    data.appendIndex(0);

    // TODO: Fix texture co-ords.
    data.appendTexCoord(QVector2D(0,0), QVector2D(1,0), QVector2D(1,1), QVector2D(0,1));

    QGLSceneNode* node = new QGLSceneNode();
    node->setGeometry(data);
    node->setStart(0);
    node->setCount(data.indexCount());
    node->setDrawingMode(QGL::Triangles);
    node->setPalette(collection);

    if ( collection->contains(PLANE_TEXTURE) )
    {
        QGLMaterial* mat = collection->material(PLANE_TEXTURE);
        node->setMaterial(mat);
        node->setBackMaterial(mat);
    }
    else
    {
        QGLTexture2D* tex = face->parentComponent()->document()->utilityTexture(PLANE_TEXTURE);

        QGLMaterial* mat = new QGLMaterial();
        mat->setAmbientColor(QColor(255,255,255));
        mat->setDiffuseColor(QColor(0,0,0));
        mat->setSpecularColor(QColor(0,0,0));
        mat->setTexture(tex);
        mat->setTextureCombineMode(QGLMaterial::Modulate);
        collection->addMaterial(mat);
        node->setMaterial(mat);
        node->setBackMaterial(mat);
    }

    return node;
}

VIEW_END_NAMESPACE
