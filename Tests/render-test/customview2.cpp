#include "customview2.h"
#include <Qt3d/QGeometryData>
#include "renderbox.h"
#include <QtDebug>

using namespace GEOMETRY_NAMESPACE;

void CustomView2::initializeGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::FlatPerVertexColor);
    
    // Set the camera to point down the positive Z axis.
    QGLCamera* cam = this->camera();
    if ( cam )
    {
        cam->setEye(QVector3D(0.0, 0.0, -10.0));    // Put us 10 units away from the origin in negative Z.
        cam->setCenter(QVector3D(0.0, 0.0, 0));     // Point us at the origin.
    }
}

CustomView2::~CustomView2()
{
    if ( m_pCullTreeNode )
    {
        delete m_pCullTreeNode;
        m_pCullTreeNode = NULL;
    }
}

void CustomView2::paintGL(QGLPainter *painter)
{
    if ( m_pCullTreeNode )
    {
        QHash<GeomDataEx*,char> list;
        renderNode(painter, m_pCullTreeNode, list);
    }
}

void CustomView2::renderNode(QGLPainter *painter, CullTreeNode *node, QHash<GeomDataEx*,char> &prevRendered)
{
    static int recursionLevel = 0;
    recursionLevel++;
    
    QString indent;
    for ( int i = 0; i < recursionLevel; i++ )
    {
        indent.append("  ");
    }
    
    Q_ASSERT(node);
    if ( !node ) return;
    
    // Call recursively first on each child.
    //qDebug("%sDrawing each child of node %p...", indent.toLatin1().constData(), node);
    for ( int i = 0; i < node->childCount(); i++ )
    {
        renderNode(painter, dynamic_cast<CullTreeNode*>(node->childAt(i)), prevRendered);
    }
    //qDebug("%sChildren drawn.", indent.toLatin1().constData());
    
    // Draw each object.
    //qDebug("%sDrawing each object of node %p...", indent.toLatin1().constData(), node);
    for ( QHash<GeomDataEx*,char>::iterator it = node->objectsBegin(); it != node->objectsEnd(); it++ )
    {
        // Don't render if we've already drawn the object.
        GeomDataEx* geom = it.key();
        if ( prevRendered.contains(geom) ) continue;
        
        // Draw the object.
        painter->begin();
        geom->draw(painter, 0, geom->indexCount());
        
        // Record the draw.
        prevRendered.insert(geom,0);
    }
    //qDebug("%sObjects drawn.", indent.toLatin1().constData());
    
    // Draw the bounding box for this node.

    //qDebug("%sCalculating bounds...", indent.toLatin1().constData());
    QBox3D bbox = node->bounds();
    QVector3D min = bbox.minimum();
    QVector3D max = bbox.maximum();
    
    // For now let's do this the painful way...
    QGeometryData geom;
    
    for ( int i = 0; i < 8; i++ )
    {
        QVector3D vec;
        
        if ( i & 1 ) vec.setX(max.x());
        else vec.setX(min.x());
        
        if ( i & 2 ) vec.setY(max.y());
        else vec.setY(min.y());
        
        if ( i & 4 ) vec.setZ(max.z());
        else vec.setZ(min.z());
        
        geom.appendVertex(vec);
        geom.appendColor(QColor4ub(255,0,0));
    }
    
    // Add indices.
    QGL::IndexArray indices;
    indices.append(0,1,1,3);
    indices.append(3,2,2,0);
    indices.append(4,5,5,7);
    indices.append(7,6,6,4);
    indices.append(0,4,1,5);
    indices.append(2,6,3,7);
    geom.appendIndices(indices);
    geom.detach();
    
    //qDebug("%sDrawing bounds...", indent.toLatin1().constData());
    //if (recursionLevel < 2) geom.draw(painter, 0, geom.indexCount(), QGL::Lines);
    
//    // TEST: Call the drawing code manually.
//    geom.upload();
//    qDebug("%sUploaded.", indent.toLatin1().constData());
//    painter->clearAttributes();
//    qDebug("%sAttributes cleared.", indent.toLatin1().constData());
//    ::glLineWidth(1.0f);
//    qDebug("%sLine width set.", indent.toLatin1().constData());
//    painter->setVertexBundle(geom.vertexBundle());
//    qDebug("%sVertex bundle set.", indent.toLatin1().constData());
//    qDebug("%sPreparing to draw lines: %d indices...", indent.toLatin1().constData(), geom.indexCount());
    
//    painter->begin();
//    painter->draw(QGL::Lines, geom.indexBuffer(), 0, geom.indexCount());
    
    painter->begin();   // Without this line the program crashes with an access violation - why?
    geom.draw(painter, 0, geom.indexCount(), QGL::Lines);
    
    //qDebug("%sLines drawn.", indent.toLatin1().constData());
    
    recursionLevel--;
    //qDebug("---");
}
