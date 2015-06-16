#include <QCoreApplication>
#include <QQmlEngine>
#include <QtQml>
#include "map/entities/mapbrush.h"
#include <QtDebug>

using namespace MODEL_NAMESPACE;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qmlRegisterType<MapVertex>("crowbar.experiments.qml", 1, 0, "Vertex");
    qmlRegisterType<MapEdge>("crowbar.experiments.qml", 1, 0, "Edge");
    qmlRegisterType<MapFace>("crowbar.experiments.qml", 1, 0, "Face");
    qmlRegisterType<MapBrush>("crowbar.experiments.qml", 1, 0, "Brush");

    QQmlEngine engine;
    QQmlComponent component(&engine);
    component.setData("import QtQuick 2.0\nimport crowbar.experiments.qml 1.0\n"
                      "Brush { vertices: [ Vertex{}, Vertex{} ] }", QUrl());

    QList<QQmlError> errors = component.errors();
    qDebug() << "Errors encountered:" << errors.count();

    foreach ( QQmlError e, errors )
    {
        qDebug() << e.toString();
    }
}
