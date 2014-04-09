#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <Qt3D/QGLView>
#include <Qt3D/QGLSceneNode>
#include <Qt3D/QGeometryData>
#include <QtDebug>

class CustomView : public QGLView
{
public:
    CustomView(QWindow *parent = 0) : QGLView(parent), node(0), geomdata(0) {}
    ~CustomView();
    
    void setScene(QGLSceneNode* n, bool viewNormals = false)
    {
        node = n;
        qDebug() << "Number of children in new scene node:" << node->allChildren().size();
        node->setOption(QGLSceneNode::ViewNormals, viewNormals);
        
        foreach (QGLSceneNode *child, node->allChildren())
        {
            child->setOption(QGLSceneNode::ViewNormals, viewNormals);
        }
    }
    QGLSceneNode* scene() const { return node; }
    
    void setGeomData(QGeometryData* data)
    {
        qDebug() << "GeomData count:" << data->count() << data->vertices() << "Index count:" << data->indexCount() << data->indices();
        geomdata = data;
    }
    QGeometryData* geomData() const { return geomdata; }
    
protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);
    void drawNormalIndicators(QGLPainter *painter, QGeometryData* data);
    
private:
    QGLSceneNode    *node;
    QGeometryData   *geomdata;
};

#endif // CUSTOMVIEW_H
