#include <QGuiApplication>
#include "customview2.h"
#include "renderbox.h"
#include "worldculltreenode.h"
#include <QList>
#include <QTime>

using namespace DATASTRUCTURES_NAMESPACE;
using namespace GEOMETRY_NAMESPACE;
  
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
#if 0
    CustomView view;
    view.resize(800, 600);
    
    QGeometryData* geom = new QGeometryData(RenderBox(QVector3D(0,0,0), QVector3D(1,1,1)).toGeomData());
    QGLSceneNode* root = new QGLSceneNode();
    root->setGeometry(*geom);
    root->setStart(0);
    root->setCount(geom->indexCount());
    
    QGLMaterial* mat = new QGLMaterial();
    mat->setAmbientColor(QColor(255,0,0));
    mat->setSpecularColor(QColor(255,255,255));
    mat->setShininess(2);
    QGLMaterialCollection* collection = new QGLMaterialCollection();
    collection->addMaterial(mat);
    
    mat = new QGLMaterial();
    mat->setAmbientColor(QColor(0,0,255));
    mat->setSpecularColor(QColor(255,255,0));
    collection->addMaterial(mat);
    
    root->setPalette(QSharedPointer<QGLMaterialCollection>(collection));
    view.setScene(root, true);
    
    view.show();
#endif
    
    CustomView2 view;
    view.resize(800, 600);
    
    CustomView2::CullTreeNode* node = new CustomView2::CullTreeNode(QBox3D(QVector3D(-5,-5,-5), QVector3D(5,5,5)));
    
    QList<GeomDataEx*> list;
    
#define OBJECT_COUNT 25
    
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    for ( int i = 0; i < OBJECT_COUNT; i++ )
    {
        float ox = ((float)(qrand() % 90) - 40.0f) / 10.0f;
        float oy = ((float)(qrand() % 90) - 40.0f) / 10.0f;
        float oz = ((float)(qrand() % 90) - 40.0f) / 10.0f;
        qDebug("Origin for box %d: %f %f %f", i+1, ox, oy, oz);
        RenderBox box(QVector3D(ox,oy,oz), QVector3D(0.5f,0.5f,0.5f));
        box.setColorMin(QColor(0,0,255));
        box.setColorMax(QColor(0,255,0));
        GeomDataEx* geomData = new GeomDataEx(box.toGeomData());
        node->addObject(geomData);
        list.append(geomData);
    }
    
    node->splitRecurse();
    view.setNode(node);
    
    view.show();
    
    int ret = app.exec();
    
    for ( int i = 0; i < list.count(); i++ )
    {
        GeomDataEx* p = list.takeFirst();
        delete p;
    }
    
    return ret;
}
