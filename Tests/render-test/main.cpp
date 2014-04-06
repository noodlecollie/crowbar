#include <QGuiApplication>
#include <Qt3D/QGLView>
#include <Qt3D/QGLSceneNode>
#include <Qt3D/QGLBuilder>
#include <Qt3D/QGLCube>
#include <QTime>
#include <QtMath>
#include "renderbox.h"
#include <QColor>
#include <QtDebug>

using namespace GEOMETRY_NAMESPACE;

class CustomView : public QGLView
{
public:
    CustomView(QWindow *parent = 0) : QGLView(parent), node(0) {}
    ~CustomView();
    
    void setScene(QGLSceneNode* n, bool viewNormals = false)
    {
        node = n;
        qDebug() << "Number of children in new scene node:" << node->allChildren().size();
        
        foreach (QGLSceneNode *child, node->allChildren())
        {
            child->setOption(QGLSceneNode::ViewNormals, viewNormals);
        }
    }
    QGLSceneNode* scene() const { return node; }
    
protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);
    
private:
    QGLSceneNode    *node;
};

void CustomView::initializeGL(QGLPainter *painter)
{
    //painter->setStandardEffect(QGL::LitMaterial);
    painter->setStandardEffect(QGL::FlatPerVertexColor);
    
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
}

void CustomView::paintGL(QGLPainter *painter)
{
    if ( node ) node->draw(painter);
}
  
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // Set up a scene.
    QGLBuilder builder;
    for ( int x = -5; x <= 5; x++ )
    {
        for ( int y = -5; y <= 5; y++ )
        {
            for ( int z = -5; z <= 5; z++ )
            {
                // Create a RenderBox at this position.
                RenderBox box(QVector3D(x,y,z), QVector3D(-0.4f,-0.4f,-0.4f), QVector3D(0.4f,0.4f,0.4f));
                box.setColorMin(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
                box.setColorMax(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
                builder.newSection();
                builder << box.toGeomData();
            }
        }
    }
    
    CustomView view;
    view.resize(800, 600);
    
    view.setScene(builder.finalizedSceneNode());
    
    view.show();
    
    return app.exec();
}
