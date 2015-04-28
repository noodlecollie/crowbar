#include "mapview.h"
#include <QGLPainter>
#include <QApplication>
#include <QtDebug>
#include <QGLCameraAnimation>
#include <QFocusEvent>
#include <QGLSceneNode>
#include "model_util.h"
#include <QtMath>
#include <QMouseEvent>
#include <QGLAbstractEffect>
#include "mapcomponent.h"
#include "mapdocumentview.h"
#include "view_util.h"

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

MapView::MapView(QWidget *parent, GLAngleCamera* defaultCamera) : PARENT_CLASSNAME(parent)
{
    m_pDefaultCamera = defaultCamera ? defaultCamera : new GLAngleCamera(this);
    initialise();
}

MapView::~MapView()
{
    delete m_pFocusBorder;
    m_pFocusBorder = NULL;
    
    delete m_pRect;
    m_pRect = NULL;
    
    delete m_pCrosshair;
    m_pCrosshair = NULL;
}

void MapView::initialise()
{
    m_bGLInitialised = false;

    m_colBackground = QColor(Qt::black);
    m_matPreRenderTransform = QMatrix4x4(); // Sets to identity.

    m_pCamera = NULL;
    m_bDrawWhenFocusLost = false;
    m_pRenderable = NULL;
    
    m_pFocusBorder = NULL;
    m_pSelectionHandles = NULL;
    m_pRect = NULL;
    m_pCrosshair = NULL;
    buildFocusBorder();
    m_bHighlightOnFocus = false;
    m_bInDrag = false;
    m_bInRightDrag = false;
    m_bInMouseOver = false;
    m_bFocusOnMouseOver = false;
    m_bDrawSelectionHandles = true;
    
    setMouseTracking(true);

    m_pDefaultCamera->setParent(this);
    m_pDefaultCamera->setFarPlane(4096);

    m_RefreshTimer.setInterval(1000/60);
    connect(&m_RefreshTimer, SIGNAL(timeout()), this, SLOT(update()));
    connect(m_pDefaultCamera, SIGNAL(viewChanged()), this, SLOT(buildSelectionHandles()));
}

// Subclasses re-implement this to perform their own initialisation too.
// Call base class implementation first, then do your own initialising.
void MapView::virtualInitGL()
{
    initializeOpenGLFunctions();

    // Set the clear colour.
    setClearColour();

    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);

    // Cull back faces.
    glEnable(GL_CULL_FACE);

    // Enable alpha blending.
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MapView::initializeGL()
{
    virtualInitGL();
    m_bGLInitialised = true;
    m_RefreshTimer.start();
}

void MapView::paintGL()
{
    if ( !isVisible() ) return; // Do we need to do this check?
    
    makeCurrent();
    
    // Clear the depth and colour buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QGLPainter painter;
    painter.begin();

    prePaintGL(&painter);   // Before camera applied.

    painter.projectionMatrix().pop();   // Why is this here??

    // Apply the co-ordinate system transform.
    painter.modelViewMatrix().push();
    painter.modelViewMatrix() *= m_matPreRenderTransform;

    painter.projectionMatrix().push();

    Q_ASSERT(camera());
    painter.setCamera(camera());

    paintGL(&painter);      // After camera applied.

    postPaintGL(&painter);

    painter.modelViewMatrix().pop();
    painter.projectionMatrix().pop();

    painter.end();
}

void MapView::prePaintGL(QGLPainter *)
{
}

void MapView::paintGL(QGLPainter *)
{
    // Update the selection bounds.
//    if ( renderable() )
//    {
//        setSelectionBounds(renderable()->selectionBounds());
//    }
}

void MapView::postPaintGL(QGLPainter *painter)
{
    bool depthTest = painter->glIsEnabled(GL_DEPTH_TEST);
    if ( depthTest ) painter->glDisable(GL_DEPTH_TEST);
    
    painter->modelViewMatrix().push();
    painter->projectionMatrix().push();
    
    painter->modelViewMatrix().setToIdentity();
    painter->projectionMatrix().setToIdentity();

    if ( m_pSelectionHandles )
    {
        m_pSelectionHandles->draw(painter);
    }
    
    if ( m_pRect )
    {
        m_pRect->draw(painter);
    }
    
    if ( m_pCrosshair )
    {
        m_pCrosshair->draw(painter);
    }
    
    if ( highlightOnFocus() && hasFocus() && m_pFocusBorder )
    {
        m_pFocusBorder->draw(painter);
    }
    
    painter->modelViewMatrix().pop();
    painter->projectionMatrix().pop();
    
    if ( depthTest ) painter->glEnable(GL_DEPTH_TEST);
}

void MapView::setClearColour()
{
    glClearColor(m_colBackground.redF(), m_colBackground.greenF(), m_colBackground.blueF(), 1.0);
}

QColor MapView::backgroundColor() const
{
    return m_colBackground;
}

void MapView::setBackgroundColor(const QColor &colour)
{
    if ( m_colBackground == colour ) return;

    m_colBackground = colour;
    if ( m_bGLInitialised )
    {
        setClearColour();
    }

    emit backgroundColorChanged(m_colBackground);
}

QSize MapView::minimumSizeHint() const
{
    return QSize(320, 240);
}

void MapView::setCoordinateTransform(const QMatrix4x4 &transform)
{
    if ( transform == m_matPreRenderTransform ) return;

    m_matPreRenderTransform = transform;
    emit coordinateTransformChanged(m_matPreRenderTransform);
    emit viewParamsChanged();
}

QMatrix4x4 MapView::coordinateTransform() const
{
    return m_matPreRenderTransform;
}

void MapView::resetCoordinateTransform()
{
    setCoordinateTransform(QMatrix4x4());
}

GLAngleCamera* MapView::camera()
{
    return m_pCamera ? m_pCamera : m_pDefaultCamera;
}

const GLAngleCamera* MapView::camera() const
{
    return m_pCamera ? m_pCamera : m_pDefaultCamera;
}

void MapView::setCamera(GLAngleCamera *camera)
{
    if ( camera == m_pCamera ) return;

    if ( m_pCamera )
    {
        disconnect(m_pCamera, SIGNAL(viewChanged()), this, SIGNAL(viewParamsChanged()));
        disconnect(m_pCamera, SIGNAL(viewChanged()), this, SLOT(buildSelectionHandles()));
    }

    m_pCamera = camera;

    if ( m_pCamera )
    {
        connect(m_pCamera, SIGNAL(viewChanged()), this, SIGNAL(viewParamsChanged()));
        connect(m_pCamera, SIGNAL(viewChanged()), this, SLOT(buildSelectionHandles()));
    }

    emit cameraChanged(m_pCamera);
    emit viewParamsChanged();
}

void MapView::resetCamera()
{
    setCamera(NULL);
}

GLAngleCamera* MapView::defaultCamera()
{
    return m_pDefaultCamera;
}

const GLAngleCamera* MapView::defaultCamera() const
{
    return m_pDefaultCamera;
}

QPoint MapView::widgetCenter() const
{
    return QPoint((int)(width()/2), (int)(height()/2));
}

void MapView::resizeGL(int w, int h)
{
    buildFocusBorder();
    buildSelectionHandles();
    
    PARENT_CLASSNAME::resizeGL(w,h);
    emit viewParamsChanged();
}

bool MapView::openGLInitialised() const
{
    return m_bGLInitialised;
}

// Does not start the animation - call startCameraAnimation() for this.
void MapView::setCameraAnimation(QGLCameraAnimation *animation)
{
    // If we currently have an animation and it's playing, stop it.
    // This will send the stop signal.
    if ( m_pCameraAnimation )
    {
        m_pCameraAnimation->stop();
    }

    // If we've been given a null animation, just reset and return.
    if ( !animation )
    {
        m_pCameraAnimation.reset();
        return;
    }

    // Set the new animation. We now own the object and the old animation is deleted.
    m_pCameraAnimation.reset(animation);

    // Connect up the animation signals.
    connect(m_pCameraAnimation.data(), SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),
            this, SLOT(cameraAnimationStateChanged(QAbstractAnimation::State,QAbstractAnimation::State)));
}

void MapView::cameraAnimationStateChanged(QAbstractAnimation::State pre, QAbstractAnimation::State post)
{
    // If the new state is Stopped, fire cameraAnimationEnd().
    if ( pre != QAbstractAnimation::Stopped && post == QAbstractAnimation::Stopped )
    {
        emit cameraAnimationEnd(m_pCameraAnimation.data());
        return;
    }

    // If the previous state was stopped and the new state is not, fire cameraAnimationStart().
    if ( pre == QAbstractAnimation::Stopped && post != QAbstractAnimation::Stopped )
    {
        emit cameraAnimationStart(m_pCameraAnimation.data());
        return;
    }
}

void MapView::startCameraAnimation()
{
    if ( !m_pCameraAnimation ) return;

    m_pCameraAnimation->start();
}

void MapView::stopCameraAnimation()
{
    if ( !m_pCameraAnimation ) return;

    m_pCameraAnimation->stop();
}

void MapView::moveCameraTo(const QVector3D &pos, float duration, const QEasingCurve &easing)
{
    QGLCameraAnimation* a = new QGLCameraAnimation();
    a->setCamera(camera());
    a->setDuration(duration);
    a->setEasingCurve(easing);

    a->setStartEye(camera()->eye());
    a->setStartCenter(camera()->center());
    a->setStartUpVector(camera()->upVector());

    const QVector3D vDir = camera()->viewVector();
    a->setEndEye(pos);
    a->setEndCenter(pos + (vDir * camera()->eyeCenterDistance()));
    a->setEndUpVector(camera()->upVector());

    setCameraAnimation(a);
    startCameraAnimation();
}

bool MapView::drawWhenFocusLost() const
{
    return m_bDrawWhenFocusLost;
}

void MapView::setDrawWhenFocusLost(bool shouldDraw)
{
    if ( shouldDraw == m_bDrawWhenFocusLost ) return;
    
    m_bDrawWhenFocusLost = shouldDraw;
    emit drawWhenFocusLostChanged(m_bDrawWhenFocusLost);
}

IRender* MapView::renderable() const
{
    return m_pRenderable;
}

void MapView::setRenderable(IRender *renderable)
{
    if ( renderable == m_pRenderable ) return;

    m_pRenderable = renderable;
    emit renderableChanged(m_pRenderable);
}

void MapView::focusInEvent(QFocusEvent *e)
{
    if ( !m_RefreshTimer.isActive() ) m_RefreshTimer.start();
    emit gainedFocus(this);
    
    PARENT_CLASSNAME::focusInEvent(e);
}

void MapView::focusOutEvent(QFocusEvent *e)
{
    if ( !m_bDrawWhenFocusLost && m_RefreshTimer.isActive() ) m_RefreshTimer.stop();
    if ( isMouseOverActive() )
    {
        m_bInMouseOver = false;
        emit mouseOverEnd();
    }
    emit lostFocus(this);
    
    PARENT_CLASSNAME::focusOutEvent(e);
}

QVector2D MapView::widgetCenterF() const
{
    return QVector2D((((float)(size().width()))/2.0f) - 0.5f,
                     (((float)(size().height()))/2.0f) - 0.5f);
}

void MapView::buildFocusBorder()
{
    if ( m_pFocusBorder )
    {
        delete m_pFocusBorder;
        m_pFocusBorder = NULL;
    }
    
    QGeometryData data;
    QVector2D vec;
    QSize s = size();
    float width = 4.0f;
    int dp = qFloor(width/2.0f);
    int dm = qCeil(width/2.0f) - 1; // This is crazy but it seems to work better than just putting dp = dm... :/
    
    vec = clientToScreen(QPoint(dp, dp));
    data.appendVertex(QVector3D(vec.x(), vec.y(), 0));
    
    vec = clientToScreen(QPoint(s.width()-1 - dm, dp));
    data.appendVertex(QVector3D(vec.x(), vec.y(), 0));
    
    vec = clientToScreen(QPoint(s.width()-1 - dm, s.height()-1 - dm));
    data.appendVertex(QVector3D(vec.x(), vec.y(), 0));
    
    vec = clientToScreen(QPoint(dp, s.height()-1 - dm));
    data.appendVertex(QVector3D(vec.x(), vec.y(), 0));
    
    data.appendColor(QColor(Qt::red), QColor(Qt::red), QColor(Qt::red), QColor(Qt::red));
    data.appendIndex(0);
    data.appendIndex(1);
    data.appendIndex(1);
    data.appendIndex(2);
    data.appendIndex(2);
    data.appendIndex(3);
    data.appendIndex(3);
    data.appendIndex(0);

    
    m_pFocusBorder = new QGLSceneNode();
    m_pFocusBorder->setGeometry(data);
    m_pFocusBorder->setStart(0);
    m_pFocusBorder->setCount(data.indexCount());
    m_pFocusBorder->setEffect(QGL::FlatPerVertexColor);
    m_pFocusBorder->setDrawingMode(QGL::Lines);
    m_pFocusBorder->setDrawingWidth(width);          // TODO: Allow us to change this?
}

bool MapView::highlightOnFocus() const
{
    return m_bHighlightOnFocus;
}

void MapView::setHighlightOnFocus(bool enabled)
{
    if ( enabled == m_bHighlightOnFocus ) return;
    
    m_bHighlightOnFocus = enabled;
    emit highlightOnFocusChanged(m_bHighlightOnFocus);
}

QVector2D MapView::clientToScreen(const QPoint &client)
{
    float w = size().width();
    float h = size().height();
    float x = client.x() + 0.5f;
    float y = client.y() + 0.5f;
    
    return QVector2D(Model_Util::remap(x, 0, w, -1, 1),
                     Model_Util::remap(y, 0, h, 1, -1));
}

QPoint MapView::screenToClient(const QVector2D &screen)
{
    float w = size().width();
    float h = size().height();
    float x = Model_Util::remap(screen.x(), -1, 1, -0.5f, w-0.5f);
    float y = Model_Util::remap(screen.y(), -1, 1, h-0.5f, -0.5f);
    x -= 0.5f;
    y -= 0.5f;
    return QPoint(qRound(x), qRound(y));
}

void MapView::setDrawRect(const QRect &rect, const QColor &col, QGLAbstractEffect *effect)
{
    delete m_pRect;
    m_pRect = NULL;
    
    if ( rect == QRect() )
    {
        return;
    }
    
    QColor c = (col == QColor() ? QColor(Qt::white) : col);
    
    // Convert client co-ords to screen co-ords.
    QVector2D ll = clientToScreen(rect.bottomLeft());
    QVector2D ur = clientToScreen(rect.topRight());
    QVector2D ul = clientToScreen(rect.topLeft());
    QVector2D lr = clientToScreen(rect.bottomRight());
    
    QGeometryData data;
    data.appendVertex(QVector3D(ll.x(), ll.y(), 0));
    data.appendVertex(QVector3D(ul.x(), ul.y(), 0));
    data.appendVertex(QVector3D(ur.x(), ur.y(), 0));
    data.appendVertex(QVector3D(lr.x(), lr.y(), 0));
    
    data.appendColor(c,c,c,c);
    data.appendIndex(0);
    data.appendIndex(1);
    data.appendIndex(2);
    data.appendIndex(3);

    m_pRect = new QGLSceneNode();
    m_pRect->setGeometry(data);
    m_pRect->setStart(0);
    m_pRect->setCount(data.indexCount());
    m_pRect->setDrawingMode(QGL::LineLoop);
    m_pRect->setDrawingWidth(1.0f);
    
    if ( !effect )
    {
        m_pRect->setEffect(QGL::FlatPerVertexColor);
    }
    else
    {
        m_pRect->setUserEffect(effect);
    }
}

void MapView::setCrosshair(const QPoint &pos, const QColor &col)
{
    QVector2D cursorPos = clientToScreen(pos);

    QGeometryData data;
    data.appendVertex(QVector3D(-1.0f, cursorPos.y(),0));
    data.appendVertex(QVector3D(1.0f, cursorPos.y(),0));
    data.appendVertex(QVector3D(cursorPos.x(), -1.0f,0));
    data.appendVertex(QVector3D(cursorPos.x(), 1.0f,0));
    
    QColor c = (col == QColor() ? QColor(Qt::white) : col);
    data.appendColor(c,c,c,c);
    data.appendIndex(0);
    data.appendIndex(1);
    data.appendIndex(2);
    data.appendIndex(3);
    
    delete m_pCrosshair;
    m_pCrosshair = new QGLSceneNode();
    m_pCrosshair->setGeometry(data);
    m_pCrosshair->setStart(0);
    m_pCrosshair->setCount(data.indexCount());
    m_pCrosshair->setEffect(QGL::FlatPerVertexColor);
    m_pCrosshair->setDrawingMode(QGL::Lines);
    m_pCrosshair->setDrawingWidth(1.0f);
}

void MapView::clearCrosshair()
{
    delete m_pCrosshair;
    m_pCrosshair = NULL;
}

bool MapView::hasDrawRect() const
{
    return m_pRect != NULL;
}

bool MapView::hasCrosshair() const
{
    return m_pCrosshair != NULL;
}

bool MapView::isBaseDragActive() const
{
    return m_bInDrag;
}

bool MapView::isRightDragActive() const
{
    return m_bInRightDrag;
}

void MapView::enterEvent(QEvent *e)
{
    m_bInMouseOver = true;
    if ( focusOnMouseOver() )
    {
        setFocus(Qt::OtherFocusReason);
    }
    emit mouseOverBegin(mapFromGlobal(QCursor::pos()));
    
    PARENT_CLASSNAME::enterEvent(e);
}

void MapView::leaveEvent(QEvent *e)
{
    m_bInMouseOver = false;
    emit mouseOverEnd();
    
    PARENT_CLASSNAME::leaveEvent(e);
}

void MapView::mousePressEvent(QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        dragMousePress(e->pos());
        return;
    }
    
    if ( e->button() == Qt::RightButton )
    {
        dragRightMousePress(e->pos());
        return;
    }

    PARENT_CLASSNAME::mousePressEvent(e);
}

void MapView::mouseMoveEvent(QMouseEvent *e)
{
    if ( m_bInMouseOver )
    {
        emit mouseOverMove(e->pos());
    }
    
    if ( e->buttons().testFlag(Qt::LeftButton) )
    {
        dragMouseMove(e->pos());
    }
    
    if ( e->buttons().testFlag(Qt::RightButton) )
    {
        dragMouseMove(e->pos());
    }

    PARENT_CLASSNAME::mouseMoveEvent(e);
}

void MapView::mouseReleaseEvent(QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        dragMouseRelease();
        return;
    }
    
    if ( e->button() == Qt::RightButton )
    {
        dragRightMouseRelease();
        return;
    }

    PARENT_CLASSNAME::mouseReleaseEvent(e);
}

void MapView::dragMousePress(const QPoint &pos)
{
    m_bInDrag = true;
    vDragBegin(pos);
}

void MapView::dragMouseMove(const QPoint &pos)
{
    vDragMove(pos);
}

void MapView::dragMouseRelease()
{
    m_bInDrag = false;
    vDragEnd();
}

void MapView::dragRightMousePress(const QPoint &pos)
{
    m_bInRightDrag = true;
    vRightDragBegin(pos);
}

void MapView::dragRightMouseMove(const QPoint &pos)
{
    vRightDragMove(pos);
}

void MapView::dragRightMouseRelease()
{
    m_bInRightDrag = false;
    vRightDragEnd();
}

void MapView::vDragBegin(const QPoint &)
{
}

void MapView::vDragMove(const QPoint &)
{
}

void MapView::vDragEnd()
{
}

void MapView::vRightDragBegin(const QPoint &)
{
}

void MapView::vRightDragMove(const QPoint &)
{
}

void MapView::vRightDragEnd()
{
}

bool MapView::isMouseOverActive() const
{
    return m_bInMouseOver;
}

IRender::RenderMode MapView::renderMode() const
{
    return IRender::Triangles;
}

QList<MapComponent*> MapView::objectsWithin(const QRect &rect)
{
    if ( !renderable() || !renderable()->supportsObjectPicking() )
        return QList<MapComponent*>();

    makeCurrent();

    // Clear the depth and colour buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QGLPainter painter;
    painter.begin();

    prePaintGL(&painter);   // Before camera applied.

    painter.projectionMatrix().pop();   // Why is this here??

    // Apply the co-ordinate system transform.
    painter.modelViewMatrix().push();
    painter.modelViewMatrix() *= m_matPreRenderTransform;

    painter.projectionMatrix().push();

    Q_ASSERT(camera());
    painter.setCamera(camera());

    QList<MapComponent*> list = renderable()->objectsWithin(rect, &painter, renderMode(), size());

    painter.modelViewMatrix().pop();
    painter.projectionMatrix().pop();

    painter.end();

    return list;
}

MapFace* MapView::faceAt(const QPoint &p)
{
    if ( !renderable() || !renderable()->supportsObjectPicking() )
        return NULL;

    makeCurrent();

    // Clear the depth and colour buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QGLPainter painter;
    painter.begin();

    prePaintGL(&painter);   // Before camera applied.

    painter.projectionMatrix().pop();   // Why is this here??

    // Apply the co-ordinate system transform.
    painter.modelViewMatrix().push();
    painter.modelViewMatrix() *= m_matPreRenderTransform;

    painter.projectionMatrix().push();

    Q_ASSERT(camera());
    painter.setCamera(camera());

    MapFace* face = renderable()->faceAt(p, &painter, renderMode(), size());

    painter.modelViewMatrix().pop();
    painter.projectionMatrix().pop();

    painter.end();

    return face;
}

bool MapView::focusOnMouseOver() const
{
    return m_bFocusOnMouseOver;
}

void MapView::setFocusOnMouseOver(bool enabled)
{
    if ( enabled == m_bFocusOnMouseOver ) return;

    m_bFocusOnMouseOver = enabled;
    emit focusOnMouseOverChanged(m_bFocusOnMouseOver);
}

// Selection bounds should be in world space.
void MapView::buildSelectionHandles()
{
    delete m_pSelectionHandles;
    m_pSelectionHandles = NULL;

    if ( !shouldShowSelectionHandles() || m_SelectionBounds.isInfinite() || m_SelectionBounds.isNull() || !m_bDrawSelectionHandles )
    {
        return;
    }

    QVector3D min = m_SelectionBounds.minimum();
    QVector3D max = m_SelectionBounds.maximum();

    QSize s = size();
    QMatrix4x4 mv = camera()->modelViewMatrix();
    QMatrix4x4 pj = camera()->projectionMatrix((float)s.width()/(float)s.height());
    
    QVector3D minProj = pj * mv * min;
    QVector3D maxProj = pj * mv * max;

    QVector3D ur(qMax<float>(minProj.x(), maxProj.x()), qMax<float>(minProj.y(), maxProj.y()), 0);
    QVector3D lr(qMax<float>(minProj.x(), maxProj.x()), qMin<float>(minProj.y(), maxProj.y()), 0);
    QVector3D ul(qMin<float>(minProj.x(), maxProj.x()), qMax<float>(minProj.y(), maxProj.y()), 0);
    QVector3D ll(qMin<float>(minProj.x(), maxProj.x()), qMin<float>(minProj.y(), maxProj.y()), 0);

    float midX = (ur.x() + ul.x())/2.0f;
    float midY = (ur.y() + lr.y())/2.0f;

    // Number of units per pixel in each axis:
    float px = 2.0f/(float)s.width();
    float py = 2.0f/(float)s.height();

    QColor col(255,255,255);
    int dim = 7;
    QVector2D handleDim = dim * QVector2D(px,py);

    m_pSelectionHandles = new QGLSceneNode();

    // Corner handles
    QGLSceneNode* nodeUR = buildSelectionHandle(QVector2D(ur.x() + px, ur.y() + py), handleDim, col);
    nodeUR->setProperty("selectionType", "++");
    m_pSelectionHandles->addNode(nodeUR);

    QGLSceneNode* nodeLR = buildSelectionHandle(QVector2D(lr.x() + px, lr.y() - py - handleDim.y()), handleDim, col);
    nodeLR->setProperty("selectionType", "+-");
    m_pSelectionHandles->addNode(nodeLR);

    QGLSceneNode* nodeUL = buildSelectionHandle(QVector2D(ul.x() - px - handleDim.x(), ul.y() + py), handleDim, col);
    nodeUL->setProperty("selectionType", "-+");
    m_pSelectionHandles->addNode(nodeUL);

    QGLSceneNode* nodeLL = buildSelectionHandle(QVector2D(ll.x() - px - handleDim.x(), ll.y() - py - handleDim.y()), handleDim, col);
    nodeLL->setProperty("selectionType", "--");
    m_pSelectionHandles->addNode(nodeLL);

    // Top handle
    QGLSceneNode* nodeTop = buildSelectionHandle(QVector2D(midX - (handleDim.x()/2.0f), ur.y() + py), handleDim, col);
    nodeTop->setProperty("selectionType", "m+");
    m_pSelectionHandles->addNode(nodeTop);

    // Bottom handle
    QGLSceneNode* nodeBottom = buildSelectionHandle(QVector2D(midX - (handleDim.x()/2.0f), lr.y() - py - handleDim.y()), handleDim, col);
    nodeBottom->setProperty("selectionType", "m-");
    m_pSelectionHandles->addNode(nodeBottom);

    // Left handle
    QGLSceneNode* nodeLeft = buildSelectionHandle(QVector2D(ul.x() - px - handleDim.x(), midY - (handleDim.y()/2.0f)), handleDim, col);
    nodeLeft->setProperty("selectionType", "-m");
    m_pSelectionHandles->addNode(nodeLeft);

    // Right handle
    QGLSceneNode* nodeRight = buildSelectionHandle(QVector2D(ur.x() + px, midY - (handleDim.y()/2.0f)), handleDim, col);
    nodeRight->setProperty("selectionType", "+m");
    m_pSelectionHandles->addNode(nodeRight);
    
    // Record the opposite anchor positions for each handle, in view units.
    nodeUR->setProperty("anchor", QVariant(ll));
    nodeUR->setProperty("basePos", QVariant(ur));

    nodeUL->setProperty("anchor", QVariant(lr));
    nodeUL->setProperty("basePos", QVariant(ul));

    nodeLL->setProperty("anchor", QVariant(ur));
    nodeLL->setProperty("basePos", QVariant(ll));

    nodeLR->setProperty("anchor", QVariant(ul));
    nodeLR->setProperty("basePos", QVariant(lr));
    
    nodeTop->setProperty("anchor", QVariant((ll + lr)/2.0f));
    nodeTop->setProperty("basePos", QVariant((ul + ur)/2.0f));

    nodeBottom->setProperty("anchor", QVariant((ul + ur)/2.0f));
    nodeBottom->setProperty("basePos", QVariant((ll + lr)/2.0f));

    nodeLeft->setProperty("anchor", QVariant((ur + lr)/2.0f));
    nodeLeft->setProperty("basePos", QVariant((ul + ll)/2.0f));

    nodeRight->setProperty("anchor", QVariant((ul + ll)/2.0f));
    nodeRight->setProperty("basePos", QVariant((ur + lr)/2.0f));
}

QGLSceneNode* MapView::buildSelectionHandle(const QVector2D &lowerLeft, const QVector2D &dimension, const QColor &col)
{
    QGeometryData data;
    QVector3D ll(lowerLeft);
    data.appendVertex(ll, ll + QVector3D(dimension.x(), 0, 0),
                      ll + QVector3D(dimension.x(), dimension.y(), 0), ll + QVector3D(0, dimension.y(), 0));
    data.appendColor(col, col, col, col);
    data.appendIndex(0);
    data.appendIndex(1);
    data.appendIndex(2);
    data.appendIndex(3);

    QGLSceneNode* node = new QGLSceneNode();
    node->setGeometry(data);
    node->setStart(0);
    node->setCount(data.indexCount());
    node->setDrawingMode(QGL::TriangleFan);
    node->setEffect(QGL::FlatPerVertexColor);
    
    node->setProperty("handleMinX", QVariant(ll.x()));
    node->setProperty("handleMaxX", QVariant(ll.x() + dimension.x()));
    node->setProperty("handleMinY", QVariant(ll.y()));
    node->setProperty("handleMaxY", QVariant(ll.y() + dimension.y()));

    return node;
}

void MapView::setSelectionBounds(const QBox3D &bounds)
{
    if ( bounds == m_SelectionBounds ) return;

    m_SelectionBounds = bounds;
    buildSelectionHandles();
}

bool MapView::pickSelectionHandle(const QPoint &p, QString &selectionType, QVector3D &anchor, QVector3D &basePos)
{
    if ( !renderable() || !renderable()->supportsObjectPicking() || !m_pSelectionHandles || !m_bDrawSelectionHandles )
        return false;

    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QGLPainter painter;
    painter.begin();
    
    // Fix the point - OpenGL origin is bottom left.
    QPoint point(p.x(), size().height() - p.y() - 1);

    QList<QGLSceneNode*> handles = m_pSelectionHandles->children();
    QGLSceneNode* handle = NULL;
    float f1 = 0, f2 = 0;
    float* pOld = &f1;
    float* pNew = &f2;
    painter.glReadPixels(point.x(), point.y(), 1, 1,
                          GL_DEPTH_COMPONENT, GL_FLOAT, pOld);

    foreach ( QGLSceneNode* node, handles )
    {
        node->draw(&painter);
        painter.glReadPixels(point.x(), point.y(), 1, 1,
                              GL_DEPTH_COMPONENT, GL_FLOAT, pNew);
        
        if ( *pOld != *pNew )
        {
            handle = node;
        }
        
        float* t = pOld;
        pOld = pNew;
        pNew = t;
    }

    painter.end();
    if ( !handle ) return false;
    
    // We look for the "selectionType" property.
    selectionType = handle->property("selectionType").toString();
    anchor = handle->property("anchor").value<QVector3D>();
    basePos = handle->property("basePos").value<QVector3D>();
    
    return true;
}

bool MapView::drawSelectionHandles() const
{
    return m_bDrawSelectionHandles;
}

void MapView::setDrawSelectionHandles(bool enabled)
{
    if ( !shouldShowSelectionHandles() )
    {
        m_bDrawSelectionHandles = false;
        buildSelectionHandles();
        return;
    }

    if ( m_bDrawSelectionHandles == enabled ) return;

    m_bDrawSelectionHandles = enabled;
    buildSelectionHandles();
    emit drawSelectionHandlesChanged(m_bDrawSelectionHandles);
}

VIEW_END_NAMESPACE
