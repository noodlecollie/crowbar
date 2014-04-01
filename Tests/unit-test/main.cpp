#include <QGuiApplication>
#include <Qt3D/QBox3D>
#include <QVector3D>
#include <QTime>
#include "worldculltreenode.h"
#include "renderbox.h"

using namespace NOctree;

class Temp : public NGeometry::IConstBBoxVolume
{
public:
    explicit Temp() {}
    
    virtual bool _implementsIConstBBoxVolume() const { return true; }
    
    virtual QBox3D boundingBox() const { return QBox3D(); }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
//    WorldCullTreeNode<IConstBBoxVolume*, 1024, 2> tree(QBox3D(QVector3D(-1024.0f, -1024.0f, -1024.0f), QVector3D(1024.0f, 1024.0f, 1024.0f)));
//    qDebug("Tree stats: %d %d", tree.splitMinDimensions(), tree.splitMinObjects());
    
//    RenderBox box(QVector3D(5.0f, 3.0f, 2.0f), QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, 1.0f, 1.0f));
//    IConstBBoxVolume* cbbv =  dynamic_cast<IConstBBoxVolume*>(&box);
//    Q_ASSERT(cbbv);
    
//    tree.addObjectRecurse(cbbv);
//    qDebug("First node that contains box: %p", tree.findObjectRecurse(cbbv));
    
    WorldCullTreeNode<Temp*, 1024, 2> tree(QBox3D(QVector3D(-1024.0f, -1024.0f, -1024.0f), QVector3D(1024.0f, 1024.0f, 1024.0f)));
    
    //return app.exec();
    return 0;
}
