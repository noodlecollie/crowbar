#include "mapview2d.h"
#include "glanglecamera.h"
#include <QGLPainter>
#include <QtDebug>
#include <QGLSceneNode>
#include <QKeyEvent>
#include <QWheelEvent>
#include "model_util.h"
#include <QMouseEvent>
#include "model_util.h"

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

MapView2D::MapView2D(QWidget* parent, GLAngleCamera* defaultCamera) :
    MapView(parent, defaultCamera ? defaultCamera : new GLAngleCamera())
{
    initialise();
}

MapView2D::~MapView2D()
{
}

void MapView2D::initialise()
{
    setFocusPolicy(Qt::StrongFocus);
    camera()->setProjectionType(QGLCamera::Orthographic);

    setZoom(6.0f);
    setMinGridPixelSpacing(4.0f);
    m_flZoomSensitivity = 1.0f;
    m_iDirection = Top;
    m_bDrawGrid = true;
    m_bDragToScroll = false;
    m_bInScrollDrag = false;
    m_LastDragPos = QPoint();
    
    connect(&m_Grid, &MapViewGrid::zoomChanged, this, &MapView2D::zoomChanged);
    connect(&m_Grid, &MapViewGrid::zoomChanged, this, &MapView2D::updateCamera);
    connect(&m_Grid, &MapViewGrid::zoomChanged, this, &MapView2D::viewParamsChanged);
    
    connect(&m_Grid, &MapViewGrid::minPixelSpacingChanged, this, &MapView2D::minGridPixelSpacingChanged);
    connect(&m_Grid, &MapViewGrid::minPixelSpacingChanged, this, &MapView2D::viewParamsChanged);
    connect(&m_Grid, &MapViewGrid::gridPowerChanged, this, &MapView2D::gridPowerChanged);
    connect(&m_Grid, &MapViewGrid::gridPowerChanged, this, &MapView2D::viewParamsChanged);
    
    connect(&m_Grid, &MapViewGrid::centerChanged, this, &MapView2D::positionChanged);
    connect(&m_Grid, &MapViewGrid::centerChanged, this, &MapView2D::updateCamera);
    connect(&m_Grid, &MapViewGrid::centerChanged, this, &MapView2D::viewParamsChanged);
    
    updateCamera();
}

void MapView2D::virtualInitGL()
{
    MapView::virtualInitGL();
}

void MapView2D::prePaintGL(QGLPainter *painter)
{
    MapView::prePaintGL(painter);

    painter->setStandardEffect(QGL::FlatColor);

    if ( m_bDrawGrid )
    {
        m_Grid.draw(painter, this);
    }
}

void MapView2D::paintGL(QGLPainter *painter)
{
    MapView::paintGL(painter);

    if ( renderable() ) renderable()->draw(painter, renderMode(), size());
}

void MapView2D::postPaintGL(QGLPainter *painter)
{
    MapView::postPaintGL(painter);
}

float MapView2D::zoom() const
{
    return m_Grid.zoom();
}

void MapView2D::setZoom(float zoom)
{
    m_Grid.setZoom(zoom);
}

void MapView2D::keyPressEvent(QKeyEvent *e)
{
    if ( !hasFocus() )
    {
        MapView::keyPressEvent(e);
        return;
    }

    if ( e->key() == Qt::Key_BracketRight&& !e->isAutoRepeat() )
    {
        setGridPower(gridPower() + 1);
        return;
    }
    
    if ( e->key() == Qt::Key_BracketLeft && !e->isAutoRepeat() )
    {
        setGridPower(gridPower() - 1);
        return;
    }

    if ( e->key() == Qt::Key_Space && !e->isAutoRepeat() )
    {
        m_bDragToScroll = true;
        return;
    }

    MapView::keyPressEvent(e);
}

void MapView2D::keyReleaseEvent(QKeyEvent *e)
{
    if ( !hasFocus() )
    {
        MapView::keyReleaseEvent(e);
        return;
    }

    if ( e->key() == Qt::Key_Space && !e->isAutoRepeat() )
    {
        m_bDragToScroll = false;
        return;
    }

    MapView::keyReleaseEvent(e);
}

void MapView2D::wheelEvent(QWheelEvent *e)
{
    if ( !hasFocus() )
    {
        MapView::wheelEvent(e);
        return;
    }

    int numDegrees = e->delta();

    bool zoomOnMousePos = e->x() >= 0 && e->x() < size().width() && e->y() >= 0 && e->y() < size().height();

    // Find the grid position under the cursor, perform the zoom, then translate so that the same
    // grid position is under the cursor again.
    QVector2D cursorPosClient(e->x(), e->y());
    
    QMatrix4x4 clientToGrid = Model_Util::clientToGrid(zoom(), size(), position());
    QVector2D cursorPosGrid = Model_Util::multiply(clientToGrid, cursorPosClient);

    if ( numDegrees < 0 )
    {
        float d = (float)(numDegrees);
        float factor = 1.0f + (d * m_flZoomSensitivity * 0.002f);
        zoomIn(factor);
    }
    else
    {
        float d = (float)(-numDegrees);
        float factor = 1.0f + (d * m_flZoomSensitivity * 0.002f);
        zoomOut(factor);
    }

    if ( zoomOnMousePos )
    {
        // Translate the grid.
        putClientPositionOnGrid(QPoint(e->x(), e->y()), cursorPosGrid);
    }

    e->accept();
}

void MapView2D::zoomIn(float scaleFactor)
{
    if ( scaleFactor == 0.0f ) return;

    setZoom(zoom() / scaleFactor);
}

float MapView2D::minGridPixelSpacing() const
{
    return m_Grid.minPixelSpacing();
}

void MapView2D::setMinGridPixelSpacing(float spacing)
{
    m_Grid.setMinPixelSpacing(spacing);
}

int MapView2D::gridPower() const
{
   return m_Grid.gridPower();
}

void MapView2D::setGridPower(int power)
{
    m_Grid.setGridPower(power);
}

float MapView2D::zoomSensitivity() const
{
    return m_flZoomSensitivity;
}

void MapView2D::setZoomSensitivity(float s)
{
    if ( s == m_flZoomSensitivity ) return;

    m_flZoomSensitivity = s;
    emit zoomSensitivityChanged(m_flZoomSensitivity);
}

MapView2D::CameraDirection MapView2D::direction() const
{
    return m_iDirection;
}

void MapView2D::setDirection(CameraDirection direction)
{
    if ( direction == m_iDirection ) return;
    
    m_iDirection = direction;
    
    // For some reason these two lines need to be left exactly as they are in order
    // for the view and grid to work out correctly, even though updateCamera() is called twice.
    // I have no idea why.
    updateCamera();
    setPosition(position());
    
    emit directionChanged(m_iDirection);
}

void MapView2D::updateCamera()
{
    // Set the camera up facing in the right direction.
    QVector3D eye, center, up;
    cameraParameters(eye, center, up);
    
    // Translate the camera to the correct position.
    QVector3D xAxis = camera()->rightVector();
    QVector3D yAxis = camera()->upVector();
    QVector3D translation = (position().x() * xAxis) + (position().y() * yAxis);

    camera()->setEye(eye + translation);
    camera()->setCenter(center + translation);
    camera()->setUpVector(up);

    camera()->setNearPlane(CAMERA2D_NEAR_PLANE_OFFSET);
    camera()->setFarPlane(CAMERA2D_NEAR_PLANE_OFFSET + (Model_Util::mapCoordMax().x() * 2.0f));
    
    // Set the camera zoom.
    // After some empirical investigation, the camera size needs to be
    // a square whose sides are zoom * view's smallest side.
    // Not sure why as of right now, but I'm sure there's a good reason.
    int dim = qMin<int>(size().width(), size().height());
    camera()->setViewSize(QSizeF(zoom() * (float)dim, zoom() * (float)dim));
}

QVector3D MapView2D::gridXAxisInWorld(CameraDirection dir)
{
    switch(dir)
    {
    case XPos: return QVector3D(0,-1,0);
    case XNeg: return QVector3D(0,1,0);
    case YPos: return QVector3D(1,0,0);
    case YNeg: return QVector3D(-1,0,0);
    case ZPos: return QVector3D(-1,0,0);
    case ZNeg: return QVector3D(1,0,0);
    }
    
    Q_ASSERT(false);
    return QVector3D();
}

QVector3D MapView2D::gridYAxisInWorld(CameraDirection dir)
{
    switch(dir)
    {
    case XPos: return QVector3D(0,0,1);
    case XNeg: return QVector3D(0,0,1);
    case YPos: return QVector3D(0,0,1);
    case YNeg: return QVector3D(0,0,1);
    case ZPos: return QVector3D(0,1,0);
    case ZNeg: return QVector3D(0,1,0);
    }
    
    Q_ASSERT(false);
    return QVector3D();
}

QVector3D MapView2D::gridZAxisInWorld(CameraDirection dir)
{
    return QVector3D::crossProduct(gridXAxisInWorld(dir), gridYAxisInWorld(dir));
}

QVector3D MapView2D::gridViewAxisInWorld(CameraDirection dir)
{
    return -gridZAxisInWorld(dir);
}

void MapView2D::cameraParameters(QVector3D &eye, QVector3D &center, QVector3D &up)
{
    eye = gridZAxisInWorld(direction()) * (Model_Util::mapCoordMax().x() + CAMERA2D_NEAR_PLANE_OFFSET);
    center = eye + gridViewAxisInWorld(direction());
    up = gridYAxisInWorld(direction());
}

QVector2D MapView2D::position() const
{
    return m_Grid.center();
}

void MapView2D::setPosition(const QVector2D &pos)
{
    m_Grid.setCenter(pos);
    updateCamera();
}

bool MapView2D::drawGrid() const
{
    return m_bDrawGrid;
}

void MapView2D::setDrawGrid(bool enabled)
{
    if ( enabled == m_bDrawGrid ) return;
    
    m_bDrawGrid = enabled;
    emit drawGridChanged(m_bDrawGrid);
}

void MapView2D::resizeGL(int w, int h)
{
    MapView::resizeGL(w,h);

    updateCamera();
}

bool MapView2D::circularCenterHandles() const
{
    // HACK: Apparently glIsEnabled is non-const...?
    MapView2D* p = const_cast<MapView2D*>(this);
    return p->glIsEnabled(GL_POINT_SMOOTH);
}

void MapView2D::setCircularCenterHandles(bool enabled)
{
    if ( enabled == circularCenterHandles() ) return;

    if ( enabled ) glEnable(GL_POINT_SMOOTH);
    else glDisable(GL_POINT_SMOOTH);

    emit circularCenterHandlesChanged(circularCenterHandles());
}

void MapView2D::translateGrid(const QPoint &p, const QVector2D &gridPos)
{
    // Translate the window position into view co-ordinates.
    QMatrix4x4 clientToView = Model_Util::clientToView(size());
    QVector2D viewPos = Model_Util::multiply(clientToView, QVector2D(p));

    QVector2D gridPosForCentre = position();
    QVector2D gridPosForViewPoint = m_Grid.viewToGridFull(viewPos);
    QVector2D grid_ViewPointToCentre = gridPosForCentre - gridPosForViewPoint;

    // viewPos should be put at gridPos,
    // so the centre should be put at:
    setPosition(gridPos + grid_ViewPointToCentre);
}

void MapView2D::putClientPositionOnGrid(const QPoint &client, const QVector2D &gridTarget)
{
    QMatrix4x4 clientToGrid = Model_Util::clientToGrid(zoom(), size(), position());

    // The client point on the current grid:
    QVector2D gridPoint = Model_Util::multiply(clientToGrid, QVector2D(client));

    // To make gridPoint equal to gridTarget, we translate by gridTarget - gridPoint.
    setPosition(position() + (gridTarget-gridPoint));
}

void MapView2D::scrollDragMousePress(const QPoint &pos)
{
    m_bInScrollDrag = true;
    m_LastDragPos = pos;
}

void MapView2D::scrollDragMouseMove(const QPoint &pos)
{
    // Determine the pixel delta since the last event.
    QVector2D delta(pos.x() - m_LastDragPos.x(),
                    pos.y() - m_LastDragPos.y());

    // Convert this to world units.
    delta = m_Grid.viewToGrid(delta);
    delta.setX(-delta.x());

    // Apply the translation.
    setPosition(position() + delta);
    
    m_LastDragPos = pos;
}

void MapView2D::scrollDragMouseRelease()
{
    m_bInScrollDrag = false;
    m_LastDragPos = QPoint();
}

void MapView2D::vDragBegin(const QPoint &pos)
{
    if ( m_bDragToScroll )
    {
        scrollDragMousePress(pos);
    }
    else
    {
        emit dragBegin(pos);
    }
}

void MapView2D::vRightDragBegin(const QPoint &pos)
{
    emit rightDragBegin(pos);
}

void MapView2D::vDragMove(const QPoint &pos)
{
    if ( m_bDragToScroll )
    {
        if ( !m_bInScrollDrag )
        {
            emit dragEnd();
            scrollDragMousePress(pos);
        }
        else scrollDragMouseMove(pos);
    }
    else
    {
        if ( m_bInScrollDrag )
        {
            scrollDragMouseRelease();
            emit dragBegin(pos);
        }
        else emit dragMove(pos);
    }
}

void MapView2D::vRightDragMove(const QPoint &pos)
{
    emit rightDragMove(pos);
}

void MapView2D::vDragEnd()
{
    // Do checks to make sure there's a specific type of drag active to end in the first place.
    if ( m_bInScrollDrag )
    {
        scrollDragMouseRelease();
    }
    else
    {
        emit dragEnd();
    }
}

void MapView2D::vRightDragEnd()
{
    emit rightDragEnd();
}

bool MapView2D::isDragActive() const
{
    return isBaseDragActive() && !m_bInScrollDrag;
}

IRender::RenderMode MapView2D::renderMode() const
{
    return IRender::Edges;
}

QMatrix4x4 MapView2D::gridToWorld(CameraDirection dir)
{
    return Model_Util::gridToWorld(gridXAxisInWorld(dir),
                                   gridYAxisInWorld(dir));
}

VIEW_END_NAMESPACE
