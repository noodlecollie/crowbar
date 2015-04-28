#include "openglwidgetv2.h"
#include <QKeyEvent>
#include <QApplication>
#include "openglcameraanimation.h"
#include <QGLPainter>
#include <QGLLightParameters>
#include <QGLLightModel>
#include "model_util.h"

using namespace VIEW_NAMESPACE;
using namespace MODEL_NAMESPACE;

OpenGLWidgetV2::OpenGLWidgetV2(const QGLFormat &format, QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f) :
    QGLWidget(format, parent, shareWidget, f)
{
    init();
}

OpenGLWidgetV2::OpenGLWidgetV2(QGLContext *context, QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f) :
    QGLWidget(context, parent, shareWidget, f)
{
    init();
}

OpenGLWidgetV2::OpenGLWidgetV2(QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f) :
    QGLWidget(parent, shareWidget, f)
{
    init();
}

OpenGLWidgetV2::~OpenGLWidgetV2()
{
    qDebug("Destruction of QOpenGLWidgetV2");
}

void OpenGLWidgetV2::init()
{
    m_bGrabMouse = false;
    setFocusPolicy(Qt::StrongFocus);
    m_flHSensitivity = 0.4f;
    m_flVSensitivity = 0.4f;
    m_bDisableCameraControl = false;
    
    m_pDefaultCamera.reset(new QGLCamera2());
    setCamera(m_pDefaultCamera.data());
    
    camera()->setAccelTime(0.2f);
    camera()->setDecelTime(0.2f);
    
    m_bInitialised = false;

    m_pDocumentRenderer = new MapDocumentRenderer(NULL, this);
    m_pOrigin = MapOriginMarker().sceneNode(IRender::Edges);

    m_pLighting = NULL;
    m_pGlobalLighting = NULL;
    
    m_RefreshTimer.setInterval(0);
    connect(&m_RefreshTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    
    m_bTransformCoordinateSystem = false;

    m_pTempBrush = NULL;
    m_bHighlightOnFocus = false;
    m_flHighlightWidth = 1.0f;
}

QSize OpenGLWidgetV2::minimumSizeHint() const
{
    return QSize(320, 240);
}

MapDocumentRenderer* OpenGLWidgetV2::documentRenderer()
{
    return m_pDocumentRenderer;
}

const MapDocumentRenderer* OpenGLWidgetV2::documentRenderer() const
{
    return m_pDocumentRenderer;
}

void OpenGLWidgetV2::setCamera(QGLCamera2 *camera)
{
    // If null was passed, reset to default camera.
    if ( !camera ) camera = m_pDefaultCamera.data();
    
    // Don't modify things if we don't need to.
    if ( camera == m_pCamera.data() ) return;
    
    // Set the camera.
    m_pCamera = camera;
}


QGLCamera2* OpenGLWidgetV2::camera() const
{
    return m_pCamera.data();
}

void OpenGLWidgetV2::initializeGL()
{
    // Set the colour buffer clear colour to black.
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);

    // Cull back faces.
    glEnable(GL_CULL_FACE);
    
    m_FrameTimer.start();
    m_RefreshTimer.start();
    m_bInitialised = true;

    delete m_pLighting;
    m_pLighting = new QGLLightParameters(this);
    m_pLighting->setAmbientColor(QColor(230,230,230));
    m_pLighting->setDiffuseColor(QColor(255,255,230));
    m_pLighting->setSpecularColor(QColor(255,255,255));
    m_pLighting->setDirection(QVector3D(2.0f, -1.0f, 3.0f).normalized());
    
    delete m_pGlobalLighting;
    m_pGlobalLighting = new QGLLightModel(this);
    m_pGlobalLighting->setAmbientSceneColor(QColor(190,190,190));
}

void OpenGLWidgetV2::resizeGL(int w, int h)
{
    // Set up the standard viewport for the new window size.
    glViewport(0, 0, w, h);
}

void OpenGLWidgetV2::paintGL()
{
    //int dt = m_FrameTimer.elapsed();
    m_FrameTimer.restart();
    
    // Clear the depth and colour buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    QGLPainter painter;
    makeCurrent();
    painter.begin();
    
    camera()->updateFromVelocity();
    
    painter.setCamera(camera());
    
    if ( m_bTransformCoordinateSystem )
    {
        painter.modelViewMatrix().push();
        painter.modelViewMatrix() *= Model_Util::HammerToOpenGL();
    }
    
    painter.setLightModel(m_pGlobalLighting);

    painter.setMainLight(m_pLighting);
    painter.setStandardEffect(QGL::LitMaterial);
    
    Q_ASSERT(m_pOrigin);
    m_pOrigin->draw(&painter);
    Q_ASSERT(m_pDocumentRenderer);
    m_pDocumentRenderer->draw(&painter, IRender::Triangles, size());

    if ( m_bTransformCoordinateSystem )
    {
        painter.modelViewMatrix().pop();
    }
    
    painter.end();

    // TODO: Work out the best way to render the edge highlight.
}

bool OpenGLWidgetV2::initialized() const
{
    return m_bInitialised;
}

void OpenGLWidgetV2::keyPressEvent(QKeyEvent *e)
{
    static QGL::DrawingMode lastDrawingMode = QGL::Triangles;

    if ( !m_bDisableCameraControl && hasFocus() )
    {
        if ( e->key() == Qt::Key_W && !e->isAutoRepeat() )
        {
            camera()->forward(true);
            return;
        }
        
        if ( e->key() == Qt::Key_A && !e->isAutoRepeat() )
        {
            camera()->left(true);
            return;
        }
        
        if ( e->key() == Qt::Key_S && !e->isAutoRepeat() )
        {
            camera()->backward(true);
            return;
        }
        
        if ( e->key() == Qt::Key_D && !e->isAutoRepeat() )
        {
            camera()->right(true);
            return;
        }
        
        if ( e->key() == Qt::Key_Q && !e->isAutoRepeat() )
        {
            camera()->up(true);
            return;
        }
        
        if ( e->key() == Qt::Key_Z && !e->isAutoRepeat() )
        {
            camera()->down(true);
            return;
        }

        if ( e->key() == Qt::Key_Up )
        {
            //camera()->subtractPitch(5.0f);
            if ( m_pTempBrush )
            {
                m_pTempBrush->hierarchyNode().setRotationDegX(
                        m_pTempBrush->hierarchyNode().rotationDegX() + 5.0f);
            }
            return;
        }
        
        if ( e->key() == Qt::Key_Down )
        {
            //camera()->addPitch(5.0f);
            if ( m_pTempBrush )
            {
                m_pTempBrush->hierarchyNode().setRotationDegX(
                        m_pTempBrush->hierarchyNode().rotationDegX() - 5.0f);
            }
            return;
        }
        
        if ( e->key() == Qt::Key_Left )
        {
            camera()->subtractYaw(5.0f);
            //camera()->tiltPanRollEye(QAngle(0.0f, -5.0f, 0.0f));
            return;
        }
        
        if ( e->key() == Qt::Key_Right )
        {
            camera()->addYaw(5.0f);
            //camera()->tiltPanRollEye(QAngle(0.0f, 5.0f, 0.0f));
            return;
        }
        
        if ( e->key() == Qt::Key_P )
        {
            toggleMouseGrab();
            return;
        }
    }
    
    if ( e->key() == Qt::Key_O && !e->isAutoRepeat() )
    {
        // Create a camera animation to use. For testing, we return to the origin position.
        // We don't need to set a camera because setCameraAnimation() does it for us.
        OpenGLCameraAnimation* anim = new OpenGLCameraAnimation();
        anim->setStartFromCamera(*camera());
        anim->setEndCenter(QVector3D(0,0,0));
        anim->setEndEye(QVector3D(0,0,10));
        anim->setEndUpVector(QVector3D(0,1,0));
        anim->setDuration(250);
        anim->setEasingCurve(QEasingCurve(QEasingCurve::InOutQuad));
        setCameraAnimation(anim);
        
        return;
    }
    
    if ( e->key() == Qt::Key_T && !e->isAutoRepeat() )
    {
        m_bTransformCoordinateSystem = !m_bTransformCoordinateSystem;
        
        return;
    }

    if ( e->key() == Qt::Key_1 && !e->isAutoRepeat() )
    {
        qDebug("Setting drawing mode to triangles.");
        m_pDocumentRenderer->setDrawingMode(QGL::Triangles);
        lastDrawingMode = QGL::Triangles;
        return;
    }

    if ( e->key() == Qt::Key_2 && !e->isAutoRepeat() )
    {
        qDebug("Setting drawing mode to points.");
        m_pDocumentRenderer->setDrawingMode(QGL::Points);
        lastDrawingMode = QGL::Points;
        m_pDocumentRenderer->setDrawingWidth(3.0f);
        return;
    }

    if ( e->key() == Qt::Key_3 && !e->isAutoRepeat() )
    {
        qDebug("Setting drawing mode to lines.");
        m_pDocumentRenderer->setDrawingMode(QGL::LineLoop);
        lastDrawingMode = QGL::LineLoop;
        m_pDocumentRenderer->setDrawingWidth(1.0f);
        return;
    }

    if ( e->key() == Qt::Key_Y && !e->isAutoRepeat() )
    {
        if ( camera()->projectionType() == QGLCamera::Perspective )
        {
            camera()->setProjectionType(QGLCamera::Orthographic);
            camera()->setCenter(camera()->eye() + QVector3D(0,-10,0));
        }
        else camera()->setProjectionType(QGLCamera::Perspective);
        return;
    }
    
    QGLWidget::keyPressEvent(e);
}

void OpenGLWidgetV2::keyReleaseEvent(QKeyEvent *e)
{
    if ( e->key() == Qt::Key_W && !e->isAutoRepeat() )
    {
        camera()->forward(false);
        return;
    }
    
    if ( e->key() == Qt::Key_A && !e->isAutoRepeat() )
    {
        camera()->left(false);
        return;
    }
    
    if ( e->key() == Qt::Key_S && !e->isAutoRepeat() )
    {
        camera()->backward(false);
        return;
    }
    
    if ( e->key() == Qt::Key_D && !e->isAutoRepeat() )
    {
        camera()->right(false);
        return;
    }
    
    if ( e->key() == Qt::Key_Q && !e->isAutoRepeat() )
    {
        camera()->up(false);
        return;
    }
    
    if ( e->key() == Qt::Key_Z && !e->isAutoRepeat() )
    {
        camera()->down(false);
        return;
    }
    
    QGLWidget::keyReleaseEvent(e);
}

void OpenGLWidgetV2::focusInEvent(QFocusEvent *e)
{
    if ( m_bGrabMouse ) QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));

    QGLWidget::focusInEvent(e);
}

void OpenGLWidgetV2::focusOutEvent(QFocusEvent *e)
{
    if ( m_bGrabMouse ) QApplication::restoreOverrideCursor();
    camera()->clearMovementFlags();
    
    QGLWidget::focusOutEvent(e);
}

void OpenGLWidgetV2::mouseMoveEvent(QMouseEvent *e)
{
    if ( m_bGrabMouse )
    {
        processMouseMovement(e->globalPos());
        return;
    }
    else
    {
        QGLWidget::mouseMoveEvent(e);
    }
}

void OpenGLWidgetV2::processMouseMovement(const QPoint &pos)
{
    // Store the mouse delta for this frame.
    QPoint delta = pos - mapToGlobal(widgetCentre());
    
    // Make sure we're not resetting the mouse if it hasn't moved at least a full pixel away!
    if ( delta == QPoint(0,0) ) return;
    
    // Perform yaw and pitch movement.
    camera()->addYaw((float)delta.x() * horizontalSensitivity());
    camera()->addPitch((float)delta.y() * verticalSensitivity());
    
    // Reset mouse position.
    moveCursorToWidgetCentre();
}

void OpenGLWidgetV2::moveCursorToWidgetCentre()
{
    QCursor::setPos(mapToGlobal(widgetCentre()));
}

QPoint OpenGLWidgetV2::widgetCentre() const
{
    return QPoint((int)(width()/2), (int)(height()/2));
}

float OpenGLWidgetV2::horizontalSensitivity() const
{
    return m_flHSensitivity;
}

void OpenGLWidgetV2::setHorizontalSensitivity(float sensitivity)
{
    m_flHSensitivity = sensitivity;
}

float OpenGLWidgetV2::verticalSensitivity() const
{
    return m_flVSensitivity;
}

void OpenGLWidgetV2::setVerticalSensitivity(float sensitivity)
{
    m_flVSensitivity = sensitivity;
}

void OpenGLWidgetV2::toggleMouseGrab()
{
    if ( !m_bGrabMouse )
    {
        m_bGrabMouse = true;
        QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));  // This is a bit of a hacky way of doing things but grabMouse() won't set the cursor properly...
        grabMouse(QCursor(Qt::BlankCursor));
        setMouseTracking(true);
        moveCursorToWidgetCentre();
    }
    else
    {
        setMouseTracking(false);
        releaseMouse();
        QApplication::restoreOverrideCursor();
        m_bGrabMouse = false;
    }
}

void OpenGLWidgetV2::setCameraAnimation(QGLCameraAnimation *animation)
{
    if ( m_pCameraAnimation ) disconnect(m_pCameraAnimation.data(), SIGNAL(finished()), this, SLOT(animationFinished()));
    m_pCameraAnimation.reset(animation);
    
    if ( m_pCameraAnimation )
    {
        // Force the concerned camera to be ours.
        m_pCameraAnimation->setCamera(camera());
        
        // Set up signals.
        connect(m_pCameraAnimation.data(), SIGNAL(finished()), this, SLOT(animationFinished()));
        
        // Disable camera control.
        m_bDisableCameraControl = true;
        
        // Start the animation.
        m_pCameraAnimation->start();
    }
    
    // If the animation has been set to a valid object, disable user camera control. Otherwise, enable it.
    m_bDisableCameraControl = !m_pCameraAnimation.isNull();
}

bool OpenGLWidgetV2::hasCameraAnimation() const
{
    return !m_pCameraAnimation.isNull();
}

void OpenGLWidgetV2::animationFinished()
{
    // Enable user camera control.
    m_bDisableCameraControl = false;
}

void OpenGLWidgetV2::setTransformCoords(bool b)
{
    m_bTransformCoordinateSystem = b;
}

bool OpenGLWidgetV2::transformCoords() const
{
    return m_bTransformCoordinateSystem;
}

bool OpenGLWidgetV2::highlight() const
{
    return m_bHighlightOnFocus;
}

void OpenGLWidgetV2::setHighlight(bool enabled)
{
    m_bHighlightOnFocus = enabled;
}

void OpenGLWidgetV2::setHighlightWidth(float width)
{
    m_flHighlightWidth = width;
}

float OpenGLWidgetV2::highlightWidth() const
{
    return m_flHighlightWidth;
}
