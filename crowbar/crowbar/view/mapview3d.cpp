#include "mapview3d.h"
#include "maporiginmarker.h"
#include "usercontrolledcamera.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QWheelEvent>
#include <QGLLightParameters>
#include <QGLPainter>

VIEW_BEGIN_NAMESPACE

MapView3D::MapView3D(QWidget* parent, GLAngleCamera* defaultCamera) :
    MapView(parent, defaultCamera ? defaultCamera : new UserControlledCamera())
{
    initialise();
}

MapView3D::~MapView3D()
{
    delete m_pOrigin;
    m_pOrigin = NULL;
}

void MapView3D::initialise()
{
    setFocusPolicy(Qt::StrongFocus);
    m_flHSensitivity = 1.0f;
    m_flVSensitivity = 1.0f;
    m_bGrabMouse = false;
    m_bDisableCameraControl = false;
    m_pOrigin = MapOriginMarker().sceneNode(IRender::Edges);

    camera()->setProjectionType(QGLCamera::Perspective);
    camera()->setFieldOfView(90.0f);

    connect(this, SIGNAL(cameraAnimationStart(QGLCameraAnimation*)), this, SLOT(disableCameraControl()));
    connect(this, SIGNAL(cameraAnimationEnd(QGLCameraAnimation*)), this, SLOT(enableCameraControl()));
}

void MapView3D::virtualInitGL()
{
    MapView::virtualInitGL();
}

void MapView3D::prePaintGL(QGLPainter *painter)
{
    MapView::prePaintGL(painter);

    if ( renderable() ) painter->setLightModel(renderable()->lightModel());
    
    QList<const QGLLightParameters*> lights;
    if ( renderable() ) lights = renderable()->lights();
    
    foreach ( const QGLLightParameters* l, lights )
    {
        painter->addLight(l);
    }
    
    userCamera()->updateFromVelocity();
}

void MapView3D::paintGL(QGLPainter *painter)
{
    MapView::paintGL(painter);
    
    m_pOrigin->draw(painter);
    if ( renderable() ) renderable()->draw(painter, renderMode(), size());
}

void MapView3D::postPaintGL(QGLPainter *painter)
{
    MapView::postPaintGL(painter);
}

float MapView3D::horizontalSensitivity() const
{
    return m_flHSensitivity;
}

void MapView3D::setHorizontalSensitivity(float sensitivity)
{
    m_flHSensitivity = sensitivity;
}

float MapView3D::verticalSensitivity() const
{
    return m_flVSensitivity;
}

void MapView3D::setVerticalSensitivity(float sensitivity)
{
    m_flVSensitivity = sensitivity;
}

void MapView3D::moveCursorToWidgetCentre()
{
    QCursor::setPos(mapToGlobal(widgetCenter()));
}

void MapView3D::processMouseMovement(const QPoint &pos)
{
    // Store the mouse delta for this frame.
    QPoint delta = pos - mapToGlobal(widgetCenter());

    // Make sure we're not resetting the mouse if it hasn't moved at least a full pixel away!
    if ( delta == QPoint(0,0) ) return;

    // Perform yaw and pitch movement.
    camera()->addYaw((float)(-delta.x()) * horizontalSensitivity());
    camera()->addPitch((float)delta.y() * verticalSensitivity());

    // Reset mouse position.
    moveCursorToWidgetCentre();
}

void MapView3D::toggleMouseGrab()
{
    if ( !m_bGrabMouse )
    {
        m_bGrabMouse = true;
        QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));  // This is a bit of a hacky way of doing things but grabMouse() won't set the cursor properly...
        grabMouse(QCursor(Qt::BlankCursor));
        setMouseTracking(true);
        moveCursorToWidgetCentre();
    }
    else if ( m_bGrabMouse )
    {
        setMouseTracking(false);
        releaseMouse();
        QApplication::restoreOverrideCursor();
        m_bGrabMouse = false;
    }
}

void MapView3D::keyPressEvent(QKeyEvent *e)
{
    if ( !hasFocus() )
    {
        MapView::keyPressEvent(e);
        return;
    }

    if ( !m_bDisableCameraControl )
    {
        if ( e->key() == Qt::Key_W && !e->isAutoRepeat() )
        {
            userCamera()->forward(true);
            return;
        }

        if ( e->key() == Qt::Key_S && !e->isAutoRepeat() )
        {
            userCamera()->backward(true);
            return;
        }

        if ( e->key() == Qt::Key_A && !e->isAutoRepeat() )
        {
            userCamera()->left(true);
            return;
        }

        if ( e->key() == Qt::Key_D && !e->isAutoRepeat() )
        {
            userCamera()->right(true);
            return;
        }

        if ( e->key() == Qt::Key_Q && !e->isAutoRepeat() )
        {
            userCamera()->up(true);
            return;
        }

        if ( e->key() == Qt::Key_E && !e->isAutoRepeat() )
        {
            userCamera()->down(true);
            return;
        }

        if ( e->key() == Qt::Key_Z && !e->isAutoRepeat() )
        {
            toggleMouseGrab();
            return;
        }
    }

    MapView::keyPressEvent(e);
}

void MapView3D::keyReleaseEvent(QKeyEvent *e)
{
    if ( !hasFocus() )
    {
        MapView::keyReleaseEvent(e);
        return;
    }

    if ( e->key() == Qt::Key_W && !e->isAutoRepeat() )
    {
        userCamera()->forward(false);
        return;
    }

    if ( e->key() == Qt::Key_A && !e->isAutoRepeat() )
    {
        userCamera()->left(false);
        return;
    }

    if ( e->key() == Qt::Key_S && !e->isAutoRepeat() )
    {
        userCamera()->backward(false);
        return;
    }

    if ( e->key() == Qt::Key_D && !e->isAutoRepeat() )
    {
        userCamera()->right(false);
        return;
    }

    if ( e->key() == Qt::Key_Q && !e->isAutoRepeat() )
    {
        userCamera()->up(false);
        return;
    }

    if ( e->key() == Qt::Key_E && !e->isAutoRepeat() )
    {
        userCamera()->down(false);
        return;
    }

    MapView::keyReleaseEvent(e);
}

void MapView3D::mouseMoveEvent(QMouseEvent *e)
{
    if ( !hasFocus() )
    {
        MapView::mouseMoveEvent(e);
    }

    if ( m_bGrabMouse )
    {
        processMouseMovement(e->globalPos());
        return;
    }
    else
    {
        MapView::mouseMoveEvent(e);
    }
}

void MapView3D::focusInEvent(QFocusEvent *e)
{
    if ( m_bGrabMouse ) QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));

    MapView::focusInEvent(e);
}

void MapView3D::focusOutEvent(QFocusEvent *e)
{
    if ( m_bGrabMouse ) QApplication::restoreOverrideCursor();
    userCamera()->clearMovementFlags();

    MapView::focusOutEvent(e);
}

void MapView3D::wheelEvent(QWheelEvent *e)
{
    if ( !hasFocus() )
    {
        MapView::wheelEvent(e);
        return;
    }

    int numDegrees = e->delta();
    QVector3D translation = camera()->viewVector() * numDegrees;
    camera()->setEye(camera()->eye() + translation);
    camera()->setCenter(camera()->center() + translation);

    e->accept();
}

bool MapView3D::cameraControl() const
{
    return !m_bDisableCameraControl;
}

void MapView3D::disableCameraControl()
{
    m_bDisableCameraControl = true;
}

void MapView3D::enableCameraControl()
{
    m_bDisableCameraControl = false;
}

UserControlledCamera* MapView3D::userCamera()
{
#ifndef QT_NO_DEBUG
    UserControlledCamera* c = qobject_cast<UserControlledCamera*>(camera());
    Q_ASSERT(c);
    return c;
#else
    return qobject_cast<UserControlledCamera*>(camera());
#endif
}

const UserControlledCamera* MapView3D::userCamera() const
{
#ifndef QT_NO_DEBUG
    const UserControlledCamera* c = qobject_cast<const UserControlledCamera*>(camera());
    Q_ASSERT(c);
    return c;
#else
    return qobject_cast<const UserControlledCamera*>(camera());
#endif
}

UserControlledCamera* MapView3D::defaultUserCamera()
{
#ifndef QT_NO_DEBUG
    UserControlledCamera* c = qobject_cast<UserControlledCamera*>(defaultCamera());
    Q_ASSERT(c);
    return c;
#else
    return qobject_cast<UserControlledCamera*>(camera());
#endif
}

const UserControlledCamera* MapView3D::defaultUserCamera() const
{
#ifndef QT_NO_DEBUG
    const UserControlledCamera* c = qobject_cast<const UserControlledCamera*>(defaultCamera());
    Q_ASSERT(c);
    return c;
#else
    return qobject_cast<const UserControlledCamera*>(camera());
#endif
}

IRender::RenderMode MapView3D::renderMode() const
{
    return IRender::Triangles;
}

void MapView3D::vDragBegin(const QPoint &p)
{
    emit dragBegin(p);
}

void MapView3D::vDragMove(const QPoint &p)
{
    emit dragMove(p);
}

void MapView3D::vDragEnd()
{
    emit dragEnd();
}

void MapView3D::vRightDragBegin(const QPoint &p)
{
    emit rightDragBegin(p);
}

void MapView3D::vRightDragMove(const QPoint &p)
{
    emit rightDragMove(p);
}

void MapView3D::vRightDragEnd()
{
    emit rightDragEnd();
}

bool MapView3D::inMouseLook() const
{
    return m_bGrabMouse;
}

void MapView3D::cancelMouseLook()
{
    if ( m_bGrabMouse ) toggleMouseGrab();
}

VIEW_END_NAMESPACE
