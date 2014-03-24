#include <QGuiApplication>
#include <Qt3D/QGLView>
#include <Qt3D/QGLSceneNode>
#include <Qt3D/QGLBuilder>
#include <Qt3D/QGLCube>
#include <QTime>
#include <QtMath>
#include "renderbox.h"

using namespace NGeometry;

class CustomView : public QGLView
{
public:
    CustomView(QWindow *parent = 0) : QGLView(parent), node(0), text(0), mat(0), tex(0), time(), box() {}
    ~CustomView();
    
protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);
    
private:
    QGLSceneNode    *node;
    QGLSceneNode    *text;
    QGLMaterial     *mat;
    QGLTexture2D    *tex;
    QTime           time;
    RenderBox       box;
};

void CustomView::initializeGL(QGLPainter *painter)
{
    box.setWOrigin(QVector3D(0.0, 0.0, 0.0));
    box.setExtents(QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, 1.0f, 1.0f));
    box.setColorMin(QColor(0,255,0));
    box.setColorMax(QColor(255,0,255));
    painter->setStandardEffect(QGL::LitMaterial);
    QGLBuilder builder;
    builder << box.toGeomData();
    node = builder.finalizedSceneNode();
    
    // Set the camera to point down the positive Z axis.
    QGLCamera* cam = this->camera();
    if ( cam )
    {
        cam->setEye(QVector3D(0.0, 0.0, -10.0));    // Put us 10 units away from the origin in negative Z.
        cam->setCenter(QVector3D(0.0, 0.0, 0));     // Point us at the origin.
    }
    
    tex = new QGLTexture2D();
    mat = new QGLMaterial();
    time = QTime::currentTime();
    time.start();
}

CustomView::~CustomView()
{
    if ( node ) delete node;
    if ( text ) delete text;
    if ( mat ) delete mat;
    if ( tex ) delete tex;
}

void CustomView::paintGL(QGLPainter *painter)
{
#if 0
    static int shift = 0;
    shift++;
    if ( shift >= 360 ) shift = 0;
    float rad = qDegreesToRadians((float)shift);
    if ( node )
    {
        float offset = sin(rad) * 4;
        QVector3D vec(0.0, 0.0, offset);
        box.setWOrigin(vec);
        QGLBuilder b;
        b << box.toGeomData();
        if ( node ) delete node;
        node = b.finalizedSceneNode();
        node->draw(painter);
    }
    if ( !text )
    {
            // build some triangles to stick the text on
            QGLBuilder b;
            b.addPane(2.0f);
            QMatrix4x4 matrix(1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 5.0,
                              0.0, 0.0, 0.0, 1.0);
            text = b.finalizedSceneNode();
            text->setLocalTransform(matrix);
    }
    //qDebug("Set text.");
    // paint the text onto an image
    QImage uv(256, 256, QImage::Format_ARGB32);
    QPainter p;
    if ( p.begin(&uv) )
    {
        p.setRenderHint(QPainter::Antialiasing);
        p.drawText(uv.rect(), Qt::AlignCenter, QString("FPS: %0").arg( (1.0f/(float)time.elapsed()) * 1000.0f ));
        time.restart();
    }
    //qDebug("Written text.");
    // put the image into a material and stick in onto the triangles
    tex->setImage(uv);
    //qDebug("Created texture.");
    mat->setTexture(tex);
    //qDebug("Set material.");
    text->setMaterial(mat);
    //qDebug("Applied text material.");
    //qDebug("Transparent: %d", mat->isTransparent());
    text->setEffect(QGL::FlatDecalTexture2D);
    //qDebug("Set text effect.");
    // paint on what ever the current context is
    text->draw(painter);
    //qDebug("Drawn text.");
#endif
    
    QGLBuilder b;
    
    QGeometryData geom = box.toGeomData();
    geom.detach();
    box.setWOrigin(QVector3D(2.0f, 2.0f, 2.0f));
    QGeometryData geom2 = box.toGeomData();
    box.setWOrigin(QVector3D(0.0f, 0.0f, 0.0f));
    
    // The problematic bit: none of these work.
    // All of them end up only displaying geom2 and not geom.
    
//    b << geom;
//    b << geom2;
    
//    b.newNode();
//    b.addTriangles(geom);
//    b.newNode();
//    b.addTriangles(geom2);
    
    b.newNode();
    b.addTriangles(geom);
    b.pushNode();
    b.addTriangles(geom2);
    
    // End
    
    if ( node ) delete node;
    node = b.finalizedSceneNode();
    node->draw(painter);
}
  
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    CustomView view;
    view.resize(800, 600);
    view.show();
    
    return app.exec();
}
