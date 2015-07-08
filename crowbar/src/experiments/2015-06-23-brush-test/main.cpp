#include <QApplication>
#include <QWindow>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DRenderer/QRenderAspect>
#include <Qt3DRenderer/QDirectionalLight>
#include <Qt3DCore/QCamera>
#include <Qt3DInput/QInputAspect>
#include <Qt3DRenderer/QFrameGraph>
#include <Qt3DRenderer/QForwardRenderer>
#include <Qt3DRenderer/QCuboidMesh>
#include "geometry/brush.h"
#include "geometry/brushvertex.h"
#include "geometry/brushface.h"
#include "rendering/brushmesh.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QFile>
#include "util/simple3dwindow.h"

using namespace MODEL_NAMESPACE;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qmlRegisterType<Brush>("crowbar.experiments.qml", 1, 0, "Brush");
    qmlRegisterType<BrushVertex>("crowbar.experiments.qml", 1, 0, "BrushVertex");
    qmlRegisterType<BrushFace>("crowbar.experiments.qml", 1, 0, "BrushFace");

#if 1
    // Create a window and an aspect engine.
    Simple3DWindow* m_pWindow = new Simple3DWindow();

    QQmlEngine engine;
    QQmlComponent component(&engine);
    QFile qmlFile(":/sample.qml");
    if ( !qmlFile.open(QIODevice::ReadOnly) )
    {
        qDebug() << "Unable to load sample QML file.";
        return 1;
    }

    component.setData(qmlFile.readAll(), QUrl());
    qmlFile.close();

    QObject* o = component.create();

    if ( !o )
    {
        QList<QQmlError> errors = component.errors();
        qDebug() << "QML parse errors encountered:" << errors.count();

        foreach ( QQmlError e, errors )
        {
            qDebug() << e.toString();
        }

        return 1;
    }

    Brush* brush = qobject_cast<Brush*>(o);
    brush->setObjectName("brush1");
    brush->setParent(m_pWindow->rootEntity());
    brush->convertFaceVertexIndices();

    BrushFace* f = brush->facesItemAt(0);
    for ( int i = 0; i < f->verticesCount(); i++ )
    {
        qDebug().nospace() << "Vertex " << i << ": " << f->verticesItemAt(i)->position();
    }

    BrushMesh* mesh = new BrushMesh();
    brush->addComponent(mesh);
    qDebug() << "BrushMesh parent:" << mesh->parentNode();

    m_pWindow->enableRootEntity();
    m_pWindow->show();

    int ret = a.exec();

    return ret;

#else

    Simple3DWindow win;

    win.show();

    return a.exec();

#endif
}
