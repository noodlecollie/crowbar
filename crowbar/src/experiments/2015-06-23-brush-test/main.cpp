#include <QApplication>
#include <Qt3DCore/Window>
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

using namespace MODEL_NAMESPACE;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qmlRegisterType<Brush>("crowbar.experiments.qml", 1, 0, "Brush");
    qmlRegisterType<BrushVertex>("crowbar.experiments.qml", 1, 0, "BrushVertex");
    qmlRegisterType<BrushFace>("crowbar.experiments.qml", 1, 0, "BrushFace");

    // Create a window and an aspect engine.
    Qt3D::Window* m_pWindow = new Qt3D::Window();
    Qt3D::QAspectEngine* m_pAspectEngine = new Qt3D::QAspectEngine();

    // Set up the aspect engine to deal with rendering tasks.
    Qt3D::QRenderAspect* render = new Qt3D::QRenderAspect();
    m_pAspectEngine->registerAspect(render);

    // Also deal with input for the camera.
    Qt3D::QInputAspect* input = new Qt3D::QInputAspect();
    m_pAspectEngine->registerAspect(input);

    // Initialise the engine. (Not sure exactly what this does)
    m_pAspectEngine->initialize();

    // Set the engine data. (Not sure exactly what this does either)
    QVariantMap data;
    data.insert(QStringLiteral("surface"), QVariant::fromValue(static_cast<QSurface *>(m_pWindow)));

    // This passes a pointer to the window under the property name "eventSource" in order for the input
    // aspect to receive key/mouse events from the window.
    data.insert(QStringLiteral("eventSource"), QVariant::fromValue(m_pWindow));
    m_pAspectEngine->setData(data);

    // Root entity - everything is parented to this
    Qt3D::QEntity* m_pRootEntity = new Qt3D::QEntity();

    // Light
    Qt3D::QDirectionalLight* light = new Qt3D::QDirectionalLight();
    light->setColor(Qt::white);
    light->setIntensity(1.5f);
    light->setDirection(QVector3D(0,-1,-1).normalized());

    m_pRootEntity->addComponent(light);

    // Camera
    Qt3D::QCamera* m_pCamera = new Qt3D::QCamera(m_pRootEntity);
    m_pCamera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    m_pCamera->setPosition(QVector3D(0, 0, 15.0f));
    m_pCamera->setUpVector(QVector3D(0, 1, 0));
    m_pCamera->setViewCenter(QVector3D(0, 0, 0));
    input->setCamera(m_pCamera);

//    Qt3D::QEntity* testCube = new Qt3D::QEntity(m_pRootEntity);
//    Qt3D::QCuboidMesh* testCubeMesh = new Qt3D::QCuboidMesh();
//    testCubeMesh->setXExtent(1);
//    testCubeMesh->setYExtent(1);
//    testCubeMesh->setZExtent(1);
//    testCubeMesh->setXYMeshResolution(QSize(2,2));
//    testCubeMesh->setXZMeshResolution(QSize(2,2));
//    testCubeMesh->setYZMeshResolution(QSize(2,2));
//    testCube->addComponent(testCubeMesh);

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
    brush->setParent(m_pRootEntity);
    brush->convertFaceVertexIndices();

    BrushFace* f = brush->facesItemAt(0);
    for ( int i = 0; i < f->verticesCount(); i++ )
    {
        qDebug().nospace() << "Vertex " << i << ": " << f->verticesItemAt(i)->position();
    }

    BrushMesh* mesh = new BrushMesh();
    brush->addComponent(mesh);
    qDebug() << "BrushMesh parent:" << mesh->parentNode();

    // FrameGraph
    Qt3D::QFrameGraph* m_pFrameGraph = new Qt3D::QFrameGraph();
    Qt3D::QForwardRenderer* m_pForwardRenderer = new Qt3D::QForwardRenderer();
    m_pForwardRenderer->setClearColor(QColor::fromRgbF(0.0, 0.5, 1.0, 1.0));
    m_pForwardRenderer->setCamera(m_pCamera);
    m_pFrameGraph->setActiveFrameGraph(m_pForwardRenderer);

    m_pRootEntity->addComponent(m_pFrameGraph);

    m_pAspectEngine->setRootEntity(m_pRootEntity);

    m_pWindow->show();

    int ret = a.exec();

    delete input;
    delete render;

    delete m_pRootEntity;
    delete m_pAspectEngine;
    delete m_pWindow;

    return ret;
}
