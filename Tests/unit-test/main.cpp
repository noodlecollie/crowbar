#include <QGuiApplication>
#include "keyvaluesnode.h"
#include <QtDebug>

using namespace DATASTRUCTURES_NAMESPACE;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    KeyValuesNode rootNode;
    
    rootNode.setKey("a_key");
    rootNode.setValue(100);
    
    qDebug() << "Root key:" << rootNode.key() << "Value:" << rootNode.value().toString() << "Has value:" << rootNode.hasValue();
    
    KeyValuesNode* child = rootNode.addChild();
    Q_ASSERT(child);
    child->setKey("i_am_a_child");
    child->setValue(QVector3D(1,2,3));
    
    qDebug() << "Post - root key:" << rootNode.key() << "Value:" << rootNode.value().toString() << "Has value:" << rootNode.hasValue();
    qDebug() << "Root's children:" << rootNode.childCount() << "Child key:" << child->key() << "Value:" << child->value() << "Has value:" << child->hasValue();
    qDebug() << "Root type:" << rootNode.constValue().type() << rootNode.constValue().typeName();
    qDebug() << "Child's type" << child->constValue().type() << child->constValue().typeName();
    
    //return app.exec();
    return 0;
}
