#include "simple3dwindow.h"
#include <Qt3DCore/QAspectEngine>
#include <Qt3DRenderer/QRenderAspect>
#include <Qt3DInput/QInputAspect>
#include <QVariantMap>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QCamera>
#include <Qt3DRenderer/QFrameGraph>
#include <Qt3DRenderer/QForwardRenderer>
#include <Qt3DRenderer/QDirectionalLight>

MODEL_BEGIN_NAMESPACE

Simple3DWindow::Simple3DWindow(QScreen *screen) : QWindow(screen)
{
    // This is copied from window.cpp from the Qt3D examples.
    setSurfaceType(QSurface::OpenGLSurface);
    resize(1024,768);
    QSurfaceFormat format;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        format.setVersion(4, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    format.setDepthBufferSize( 24 );
    format.setSamples( 4 );
    setFormat(format);
    create();

    initialise();
}

Simple3DWindow::~Simple3DWindow()
{
    delete m_pAspectEngine;
}

void Simple3DWindow::initialise()
{
    m_pAspectEngine = new Qt3D::QAspectEngine();

    m_pRenderAspect = new Qt3D::QRenderAspect();
    m_pAspectEngine->registerAspect(m_pRenderAspect);

    m_pInputAspect = new Qt3D::QInputAspect();
    m_pAspectEngine->registerAspect(m_pInputAspect);

    m_pAspectEngine->initialize();

    QVariantMap data;
    data.insert(QStringLiteral("surface"), QVariant::fromValue(static_cast<QSurface*>(this)));
    data.insert(QStringLiteral("eventSource"), QVariant::fromValue(this));
    m_pAspectEngine->setData(data);

    m_pRootEntity = new Qt3D::QEntity();

    initLights();

    m_pCamera = new Qt3D::QCamera(m_pRootEntity);
    initCamera();
    m_pInputAspect->setCamera(m_pCamera);

    initNodes();

    m_pFrameGraph = new Qt3D::QFrameGraph();
    m_pForwardRenderer = new Qt3D::QForwardRenderer();

    initRenderer();

    m_pForwardRenderer->setCamera(m_pCamera);
    m_pFrameGraph->setActiveFrameGraph(m_pForwardRenderer);
    m_pRootEntity->addComponent(m_pFrameGraph);
}

void Simple3DWindow::shutdown()
{
    delete m_pAspectEngine;
}

void Simple3DWindow::initLights()
{
    Qt3D::QDirectionalLight* light = new Qt3D::QDirectionalLight();
    light->setColor(Qt::white);
    light->setIntensity(1.5f);
    light->setDirection(QVector3D(0,-1,-1).normalized());
    m_pRootEntity->addComponent(light);
}

void Simple3DWindow::initNodes()
{
}

void Simple3DWindow::initRenderer()
{
    m_pForwardRenderer->setClearColor(QColor::fromRgbF(0.0, 0.5, 1.0, 1.0));
    m_pForwardRenderer->setCamera(m_pCamera);
    m_pFrameGraph->setActiveFrameGraph(m_pForwardRenderer);
}

void Simple3DWindow::initCamera()
{
    m_pCamera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    m_pCamera->setPosition(QVector3D(0, 0, 40.0f));
    m_pCamera->setUpVector(QVector3D(0, 1, 0));
    m_pCamera->setViewCenter(QVector3D(0, 0, 0));
}

Qt3D::QEntity* Simple3DWindow::rootEntity() const
{
    return m_pRootEntity;
}

void Simple3DWindow::enableRootEntity()
{
    m_pAspectEngine->setRootEntity(m_pRootEntity);
}

MODEL_END_NAMESPACE
