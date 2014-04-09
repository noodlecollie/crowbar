#include "customview.h"

void CustomView::initializeGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    //painter->setStandardEffect(QGL::FlatPerVertexColor);
    
    // Set the camera to point down the positive Z axis.
    QGLCamera* cam = this->camera();
    if ( cam )
    {
        cam->setEye(QVector3D(0.0, 0.0, -10.0));    // Put us 10 units away from the origin in negative Z.
        cam->setCenter(QVector3D(0.0, 0.0, 0));     // Point us at the origin.
    }
}

CustomView::~CustomView()
{
    if ( node ) delete node;
    if ( geomdata ) delete geomdata;
}

void CustomView::paintGL(QGLPainter *painter)
{
    // If there's a node, that takes priority.
    if ( node )
    {
        painter->setFaceMaterial(QGL::AllFaces, node->palette().data()->material(0));
        node->draw(painter);
    }
    else if ( geomdata )
    {
        geomdata->draw(painter, 0, geomdata->indexCount());
        drawNormalIndicators(painter, geomdata);
    }
}

void CustomView::drawNormalIndicators(QGLPainter *painter, QGeometryData* data)
{
    if ( !data ) return;
    
    // To note from this evening's experience: count() specifies number of raw vertices, indexCount() specifies number of indices.
    // If we're wanting to render an entire mesh we'll want to go up to indexCount(), not count().
    
    QVector3DArray verts;
    QArray<QColor4ub> colours;
    for (int i = 0; i < data->count(); ++i)
    {
        QVector3D a = data->vertexAt(i);
        QVector3D b = a + data->normalAt(i);
        verts.append(a, b);
        colours.append(QColor4ub(255,0,0));
        colours.append(QColor4ub(0,255,0));
    }
    
    painter->clearAttributes();
    painter->setVertexAttribute(QGL::Position, QGLAttributeValue(verts));
    painter->setVertexAttribute(QGL::Color, QGLAttributeValue(colours));
    glLineWidth(1.0f);
}
