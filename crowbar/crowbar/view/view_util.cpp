#include "view_util.h"
#include <QGLSceneNode>
#include <QtDebug>
#include <QGLBuilder>
#include <QGraphicsRotation3D>
#include <QGraphicsScale3D>
#include <QGraphicsTranslation3D>
#include "model_util.h"
#include <QQuaternion>
#include <QtMath>

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

namespace View_Util
{
    QGLSceneNode* sceneNodeForWireframe(const QGeometryData &data)
    {
        QGLSceneNode* node = new QGLSceneNode();

        node->setGeometry(data);
        node->setStart(0);
        node->setCount(data.count());
        node->setEffect(QGL::FlatPerVertexColor);
        node->setDrawingMode(QGL::Lines);
        node->setDrawingWidth(1.0f);

        return node;
    }
    
    QGLSceneNode* solidBox(const QBox3D &box, const QColor &col)
    {
        QVector3D min = box.minimum();
        QVector3D max = box.maximum();
        
        QGeometryData topExtrude;
        topExtrude.appendVertex(QVector3D(min.x(), min.y(), max.z()));
        topExtrude.appendVertex(QVector3D(min.x(), max.y(), max.z()));
        topExtrude.appendVertex(max);
        topExtrude.appendVertex(QVector3D(max.x(), min.y(), max.z()));
        topExtrude.appendVertex(QVector3D(min.x(), min.y(), max.z()));
        topExtrude.appendIndex(0);
        topExtrude.appendIndex(1);
        topExtrude.appendIndex(2);
        topExtrude.appendIndex(3);
        topExtrude.appendIndex(4);
        
        QGeometryData top;
        top.appendVertex(QVector3D(min.x(), min.y(), max.z()));
        top.appendVertex(QVector3D(min.x(), max.y(), max.z()));
        top.appendVertex(max);
        top.appendVertex(QVector3D(max.x(), min.y(), max.z()));
        top.appendColor(col, col, col, col);
        top.appendIndex(0);
        top.appendIndex(1);
        top.appendIndex(2);
        top.appendIndex(3);
        
        QGeometryData bottomExtrude(topExtrude.translated(QVector3D(0, 0, max.z() - min.z())));
        QGeometryData bottom(top.translated(QVector3D(0, 0, max.z() - min.z())));
        
        QGLBuilder builder;
        builder.addQuadsInterleaved(topExtrude, bottomExtrude);
        builder.addQuads(top);
        builder.addQuads(bottom.reversed());
        
        return builder.finalizedSceneNode();
    }
    
    QGLSceneNode* wireframeBox(const QBox3D &box, const QColor &col)
    {
        QVector3D min = box.minimum();
        QVector3D max = box.maximum();
        
        QGeometryData data;
        
        QVector3D v0(min);
        QVector3D v1(min.x(), max.y(), min.z());
        QVector3D v2(max.x(), max.y(), min.z());
        QVector3D v3(max.x(), min.y(), min.z());
        data.appendVertex(v0, v1, v2, v3);
        data.appendColor(col, col, col, col);

        QVector3D v4(max.x(), min.y(), max.z());
        QVector3D v5(max);
        QVector3D v6(min.x(), max.y(), max.z());
        QVector3D v7(min.x(), min.y(), max.z());
        data.appendVertex(v4, v5, v6, v7);
        data.appendColor(col, col, col, col);
        
        // Top edges
        data.appendIndex(0);
        data.appendIndex(1);
        
        data.appendIndex(1);
        data.appendIndex(2);
        
        data.appendIndex(2);
        data.appendIndex(3);
        
        data.appendIndex(3);
        data.appendIndex(0);
        
        // Bottom edges
        data.appendIndex(4);
        data.appendIndex(5);
        
        data.appendIndex(5);
        data.appendIndex(6);
        
        data.appendIndex(6);
        data.appendIndex(7);
        
        data.appendIndex(7);
        data.appendIndex(4);
        
        // Connecting edges
        data.appendIndex(0);
        data.appendIndex(7);
        
        data.appendIndex(1);
        data.appendIndex(6);
        
        data.appendIndex(2);
        data.appendIndex(5);
        
        data.appendIndex(3);
        data.appendIndex(4);
        
        QGLSceneNode* node = new QGLSceneNode();
        node->setGeometry(data);
        node->setStart(0);
        node->setCount(data.indexCount());
        node->setDrawingWidth(1.0f);
        node->setDrawingMode(QGL::Lines);
        
        return node;
    }
    
    QGLSceneNode* simpleOriginMarker(float scale, const QVector3D position)
    {
        QGLSceneNode* node = new QGLSceneNode();
        
        QGeometryData data;
        
        QVector3D x(scale, 0, 0);
        QVector3D y(0, scale, 0);
        QVector3D z(0, 0, scale);
        
        data.appendVertex(position, position + x);
        data.appendColor(QColor(255,0,0), QColor(255,0,0));
        data.appendVertex(position, position + y);
        data.appendColor(QColor(0,255,0), QColor(0,255,0));
        data.appendVertex(position, position + z);
        data.appendColor(QColor(0,0,255), QColor(0,0,255));
        
        data.appendIndex(0);
        data.appendIndex(1);
        data.appendIndex(2);
        data.appendIndex(3);
        data.appendIndex(4);
        data.appendIndex(5);
        
        node->setGeometry(data);
        node->setStart(0);
        node->setCount(data.indexCount());
        node->setDrawingMode(QGL::Lines);
        node->setDrawingWidth(1.0f);
        node->setEffect(QGL::FlatPerVertexColor);
        
        return node;
    }
    
    static QGLSceneNode* solidOriginMarkerComponent(const QColor &colour)
    {
        QGeometryData data;
        
        data.appendVertex(QVector3D(0,0,0));
        data.appendVertex(QVector3D(0.25f,0,0.25f), QVector3D(0.25f,0.25f,0), QVector3D(0.25f,0,-0.25f), QVector3D(0.25f,-0.25f,0));
        data.appendVertex(QVector3D(1,0,0));
        
        data.appendColor(colour);
        data.appendColor(colour, colour, colour, colour);
        data.appendColor(colour);
        
        data.appendIndex(0);
        data.appendIndex(1);
        data.appendIndex(2);
        
        data.appendIndex(0);
        data.appendIndex(2);
        data.appendIndex(3);
        
        data.appendIndex(0);
        data.appendIndex(3);
        data.appendIndex(4);
        
        data.appendIndex(0);
        data.appendIndex(4);
        data.appendIndex(1);
        
        data.appendIndex(1);
        data.appendIndex(5);
        data.appendIndex(2);
        
        data.appendIndex(2);
        data.appendIndex(5);
        data.appendIndex(3);
        
        data.appendIndex(3);
        data.appendIndex(5);
        data.appendIndex(4);
        
        data.appendIndex(4);
        data.appendIndex(5);
        data.appendIndex(1);
        
        QGLSceneNode* node = new QGLSceneNode();
        node->setGeometry(data);
        node->setStart(0);
        node->setCount(data.indexCount());
        node->setDrawingMode(QGL::Triangles);
        node->setEffect(QGL::FlatPerVertexColor);
        
        return node;
    }
    
    QGLSceneNode* solidOriginMarker(float scale, const QVector3D position)
    {
        QGLSceneNode* node = new QGLSceneNode();
        
        QGLSceneNode* xAxis = solidOriginMarkerComponent(QColor(255,0,0));
        QGraphicsScale3D* trScale = new QGraphicsScale3D(xAxis);
        trScale->setScale(QVector3D(scale,scale,scale));
        xAxis->addTransform(trScale);
        node->addNode(xAxis);
        
        QGLSceneNode* yAxis = solidOriginMarkerComponent(QColor(0,255,0));
        trScale = new QGraphicsScale3D(yAxis);
        trScale->setScale(QVector3D(scale,scale,scale));
        yAxis->addTransform(trScale);
        QGraphicsRotation3D* trRot = new QGraphicsRotation3D(yAxis);
        trRot->setAxis(QVector3D(0,0,1));
        trRot->setAngle(90.0f);
        yAxis->addTransform(trRot);
        node->addNode(yAxis);
        
        QGLSceneNode* zAxis = solidOriginMarkerComponent(QColor(0,0,255));
        trScale = new QGraphicsScale3D(zAxis);
        trScale->setScale(QVector3D(scale,scale,scale));
        zAxis->addTransform(trScale);
        trRot = new QGraphicsRotation3D(zAxis);
        trRot->setAxis(QVector3D(0,1,0));
        trRot->setAngle(-90.0f);
        zAxis->addTransform(trRot);
        node->addNode(zAxis);
        
        QGraphicsTranslation3D* translation = new QGraphicsTranslation3D(node);
        translation->setTranslate(position);
        node->addTransform(translation);
        
        return node;
    }
    
    QList<QVector2D> textureCoordinates(const FaceTextureInfo &info, const QList<QVector3D> &vertices)
    {
        if ( !info.isValid() || !info.isTexturePlaneValid() )
        {
            if ( !info.isValid() )
            {
                qWarning() << "Texture info is not valid for generating texture co-ordinates.\n"
                           << "U axis:" << info.textureUAxis() << "V axis:" << info.textureVAxis();
            }
            else
            {
                qWarning() << "Texture plane is not valid for generating texture co-ordinates."
                           << "U axis:" << info.textureUAxis() << "V axis:" << info.textureVAxis();
            }
            QList<QVector2D> out;
            for ( int i = 0; i < vertices.count(); i++ )
            {
                out.append(QVector2D());
            }
            
            return out;
        }

        // TODO: Find a nicer way of doing the following.
        // Really we want to be able to use matrices on all vectors at once.
        
        // Generate the texture plane.
        FaceTextureInfo::TexturePlane plane = info.texturePlane();

        // Set origin.
        // NOTE: For some reason X is negated in Hammer - fix this here so that we correspond.
        plane.origin = (-info.translation().x() * plane.uAxis) + (info.translation().y() * plane.vAxis);
        
        // Scale.
        plane.uAxis /= info.scale().x();
        plane.vAxis /= info.scale().y();
        plane.origin.setX(plane.origin.x() * info.scale().x());
        plane.origin.setY(plane.origin.y() * info.scale().y());

        // Rotate.
        QVector3D normal = plane.normal();
        QQuaternion rotation = QQuaternion::fromAxisAndAngle(normal, info.rotation());
        plane.uAxis = rotation.rotatedVector(plane.uAxis);
        plane.vAxis = rotation.rotatedVector(plane.vAxis);
        plane.origin = rotation.rotatedVector(plane.origin);
        
        QList<QVector2D> list;
        QMatrix4x4 txl2txc = Model_Util::texelCornerToTextureCoordinate(plane.textureSize.width(), plane.textureSize.height());

        // For each vector:
        foreach ( QVector3D vec, vertices )
        {
            // Calculate the point in 3D "plane space".
            QVector3D tempPoint = toPlaneSpace(vec, plane);
            
            // Calculate the point on the plane.
            QVector2D planePoint(tempPoint.x(), tempPoint.y());
            
            // Convert it to texture co-ordinate space.
            list.append(Model_Util::multiply(txl2txc, planePoint));
        }
        
        return list;
    }
    
    QVector3D toPlaneSpace(const QVector3D &vec, const FaceTextureInfo::TexturePlane &plane)
    {
        QVector3D localvec = vec - plane.origin;
        QVector3D normal = plane.normal();

        /*return plane.origin +
                ((QVector3D::dotProduct(plane.uAxis, localvec)) * uNorm) +
                ((QVector3D::dotProduct(plane.vAxis, localvec)) * vNorm) +
                ((QVector3D::dotProduct(normal, vec - plane.origin)) * normal);*/

        // We only want the coefficients, not the actual position.
        return QVector3D(
                    QVector3D::dotProduct(plane.uAxis, localvec),
                    QVector3D::dotProduct(plane.vAxis, localvec),
                    QVector3D::dotProduct(normal, localvec)
                    );
    }
}

VIEW_END_NAMESPACE
