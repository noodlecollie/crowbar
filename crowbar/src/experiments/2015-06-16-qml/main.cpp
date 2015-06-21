#include <QCoreApplication>
#include <QQmlEngine>
#include <QtQml>
#include <QtDebug>
#include "qmlclass.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    qmlRegisterType<QMLClass>("crowbar.experiments.qml", 1, 0, "QMLClass");
    
//    // NOTE: ALWAYS use fully qualified namespaces when interfacing with QML, as problems
//    // may occur otherwise!
//    qmlRegisterType<MODEL_NAMESPACE::MapVertex>("crowbar.experiments.qml", 1, 0, "Vertex");
//    qmlRegisterType<MODEL_NAMESPACE::MapEdge>("crowbar.experiments.qml", 1, 0, "Edge");
//    qmlRegisterType<MODEL_NAMESPACE::MapFace>("crowbar.experiments.qml", 1, 0, "Face");
//    qmlRegisterType<MODEL_NAMESPACE::MapBrush>("crowbar.experiments.qml", 1, 0, "Brush");

//    QQmlEngine engine;
//    QQmlComponent component(&engine);
//    component.setData("import QtQuick 2.0\nimport crowbar.experiments.qml 1.0\n"
//                      "Brush { vertices: [ Vertex{}, Vertex{} ] }", QUrl());
    
//    QObject* o = component.create();

//    if ( !o )
//    {
//        QList<QQmlError> errors = component.errors();
//        qDebug() << "Errors encountered:" << errors.count();
    
//        foreach ( QQmlError e, errors )
//        {
//            qDebug() << e.toString();
//        }
        
//        return 0;
//    }
    
//    MODEL_NAMESPACE::MapBrush* c = qobject_cast<MODEL_NAMESPACE::MapBrush*>(o);
//    qDebug() << "Number of properties:" << c->verticesCount();
    
    return 0;
}
