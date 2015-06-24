#include <QCoreApplication>
#include <QQmlEngine>
#include <QtQml>
#include <QtDebug>
#include "qmlclass.h"
#include "geometry/brush.h"
#include "geometry/brushface.h"
#include "geometry/brushvertex.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qmlRegisterType<QMLClass>("crowbar.experiments.qml", 1, 0, "QMLClass");
    
    qmlRegisterType<MODEL_NAMESPACE::Brush>("crowbar.experiments.qml", 1, 0, "Brush");
    qmlRegisterType<MODEL_NAMESPACE::BrushVertex>("crowbar.experiments.qml", 1, 0, "BrushVertex");
    qmlRegisterType<MODEL_NAMESPACE::BrushFace>("crowbar.experiments.qml", 1, 0, "BrushFace");

    QQmlEngine engine;
    QQmlComponent component(&engine);
    component.setData("import QtQuick 2.0\n"
                      "import crowbar.experiments.qml 1.0\n"
                      "Brush {\n"
                      "vertices: [ BrushVertex{}, BrushVertex{}, BrushVertex{} ]\n"
                      "faces: [\n"
                      "BrushFace{ vertexIndices: [ 0,1,2 ] }\n"
                      "]}"
                      , QUrl());
    
    QObject* o = component.create();

    if ( !o )
    {
        QList<QQmlError> errors = component.errors();
        qDebug() << "Errors encountered:" << errors.count();
    
        foreach ( QQmlError e, errors )
        {
            qDebug() << e.toString();
        }
        
        return 0;
    }
    
    MODEL_NAMESPACE::Brush* b = qobject_cast<MODEL_NAMESPACE::Brush*>(o);
    qDebug() << "Number of vertices:" << b->verticesCount();

    MODEL_NAMESPACE::BrushFace* f = b->facesItemAt(0);
    qDebug() << "Number of vertices specified in face:" << f->vertexIndices().value<QVector<int> >().count();

    b->convertFaceVertexIndices();
    qDebug() << "Number of physical vertex pointers converted from the indices:" << f->verticesCount();
    
    return 0;
}
