#include "mapviewgrid.h"
#include <QtMath>
#include <QGLSceneNode>
#include "model_util.h"
#include <QGLPainter>
#include "mapview2d.h"
#include "model_util.h"

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

MapViewGrid::MapViewGrid(QObject *parent) :
    QObject(parent)
{
    m_bNeedsUpdate = true;

    m_colMajor = QColor(100, 46, 0);
    m_colMinor = QColor(119,119,119);
    m_colOrigin = QColor(64,119,119);
    m_colStd = QColor(65,65,65);

    m_matGridCentre.setToIdentity();
    m_matZoom.setToIdentity();
    
    m_flMinPixelSpacing = 1.0f;

    // 64-unit gridlines.
    m_iPowerTwo = 6;

    m_pNode = NULL;
    m_Size = QSize();
}

QColor MapViewGrid::majorColor() const
{
    return m_colMajor;
}

void MapViewGrid::setMajorColor(const QColor &col)
{
    if ( m_colMajor == col ) return;

    m_colMajor = col;
    m_bNeedsUpdate = true;
    emit majorColorChanged(m_colMajor);
}

QColor MapViewGrid::minorColor() const
{
    return m_colMinor;
}

void MapViewGrid::setMinorColor(const QColor &col)
{
    if ( m_colMinor == col ) return;

    m_colMinor = col;
    m_bNeedsUpdate = true;
    emit minorColorChanged(m_colMinor);
}

QColor MapViewGrid::originColor() const
{
    return m_colOrigin;
}

void MapViewGrid::setOriginColor(const QColor &col)
{
    if ( m_colOrigin == col ) return;

    m_colOrigin = col;
    m_bNeedsUpdate = true;
    emit originColorChanged(m_colOrigin);
}

QColor MapViewGrid::standardColor() const
{
    return m_colStd;
}

void MapViewGrid::setStandardColor(const QColor &col)
{
    if ( m_colStd == col ) return;

    m_colStd = col;
    m_bNeedsUpdate = true;
    emit standardColorChanged(m_colStd);
}

int MapViewGrid::gridPower() const
{
    return m_iPowerTwo;
}

void MapViewGrid::setGridPower(int power)
{
    if ( power < 0 ) power = 0;
    else if ( power > 10 ) power = 10;

    if ( m_iPowerTwo == power ) return;

    m_iPowerTwo = power;
    m_bNeedsUpdate = true;
    emit gridPowerChanged(m_iPowerTwo);
}

QVector2D MapViewGrid::center() const
{
    return QVector2D(m_matGridCentre(0,3), m_matGridCentre(1,3));
}

void MapViewGrid::queueUpdate()
{
    regenerateTransforms();
    m_bNeedsUpdate = true;
}

void MapViewGrid::setCenter(const QVector2D &centre)
{
    if ( centre.x() == m_matGridCentre(0,3) && centre.y() == m_matGridCentre(1,3) ) return;

    m_matGridCentre = viewToGridTransform(centre);
    
    if ( m_matGridCentre(0,3) < Model_Util::mapCoordMin().x() ) m_matGridCentre(0,3) = Model_Util::mapCoordMin().x();
    else if ( m_matGridCentre(0,3) > Model_Util::mapCoordMax().x() ) m_matGridCentre(0,3) = Model_Util::mapCoordMax().x();
    
    if ( m_matGridCentre(1,3) < Model_Util::mapCoordMin().y() ) m_matGridCentre(1,3) = Model_Util::mapCoordMin().y();
    else if ( m_matGridCentre(1,3) > Model_Util::mapCoordMax().y() ) m_matGridCentre(1,3) = Model_Util::mapCoordMax().y();
    
    queueUpdate();
    emit centerChanged(center());
}

float MapViewGrid::zoom() const
{
    return m_matZoom(0,0);
}

void MapViewGrid::setZoom(float z)
{
    if ( z == m_matZoom(0,0) ) return;

    if ( z < GRID_ZOOM_MIN ) z = GRID_ZOOM_MIN;
    else if ( z > GRID_ZOOM_MAX ) z = GRID_ZOOM_MAX;
    
    m_matZoom = viewToGridTransform(z);
    queueUpdate();
    emit zoomChanged(zoom());
}

void MapViewGrid::regenerateTransforms()
{
    m_matViewGridTransform = m_matGridCentre * m_matZoom;
    m_matGridViewTransform = m_matViewGridTransform.inverted();
}

QGLSceneNode* MapViewGrid::grid(const MapView2D *view) const
{
    if ( !view ) return NULL;

    if ( m_bNeedsUpdate || !m_pNode || view->size() != m_Size )
    {
        m_Size = view->size();
        delete m_pNode;             // FIX: Getting massive memory leaks, this could have been why.
        m_pNode = buildNode(view);
        m_bNeedsUpdate = false;
    }

    return m_pNode;
}

QGLSceneNode* MapViewGrid::buildNode(const MapView2D *view) const
{
    QGLSceneNode* node = new QGLSceneNode();
    node->setGeometry(gridlines(view));
    node->setCount(node->geometry().count());
    node->setDrawingMode(QGL::Lines);
    node->setDrawingWidth(1.0f);
    node->setEffect(QGL::FlatPerVertexColor);
    return node;
}

QVector2D MapViewGrid::viewToGrid(const QVector2D &vec) const
{
    return vec4ToVec2(m_matZoom * vec2ToVec4(vec));
}

QVector2D MapViewGrid::gridToView(const QVector2D &vec) const
{
    return vec4ToVec2(m_matZoom.inverted() * vec2ToVec4(vec));
}

QVector2D MapViewGrid::viewToGridFull(const QVector2D &vec) const
{
    return vec4ToVec2(m_matViewGridTransform * vec2ToVec4(vec));
}

QVector2D MapViewGrid::gridToViewFull(const QVector2D &vec) const
{
    return vec4ToVec2(m_matGridViewTransform * vec2ToVec4(vec));
}

QGeometryData MapViewGrid::gridlines(const MapView2D * /*view*/) const
{
    if ( m_Size.isEmpty() ) return QGeometryData();

    // Delta from centre of view to max point in client space.
    QVector2D delta((float)m_Size.width() / 2.0f, (float)m_Size.height() / 2.0f);

    // The centre of the view.
    //QVector2D viewCentre = view->widgetCenterF();
    
    // Min and max points in view co-ordinates.
    QVector2D min = viewToGridFull(-delta);
    QVector2D max = viewToGridFull(delta);

    // Check whether min or max are outside the map co-ordinate bounds.
    // If so, we don't want to draw the grid outside these bounds.
    if ( min.x() < Model_Util::mapCoordMin().x() ) min.setX(Model_Util::mapCoordMin().x());
    if ( min.y() < Model_Util::mapCoordMin().y() ) min.setY(Model_Util::mapCoordMin().y());
    if ( max.x() > Model_Util::mapCoordMax().x() ) max.setX(Model_Util::mapCoordMax().x());
    if ( max.y() > Model_Util::mapCoordMax().y() ) max.setY(Model_Util::mapCoordMax().y());

    // We need to translate max and min back into view co-ordinates so that we can draw between them.
    QVector2D minViewCoord = gridToViewFull(min);
    QVector2D maxViewCoord = gridToViewFull(max);
    //minViewCoord += viewCentre;
    //maxViewCoord += viewCentre;

    QVector2D minScreen = viewToScreen(minViewCoord);
    QVector2D maxScreen = viewToScreen(maxViewCoord);

    // Find out where we should draw between in OpenGL screen space.

    // Calculate our minimum grid spacing (ie the amount we need to increment between grid lines).
    int minGrid = (int)qPow(2, gridPower());

    // Check the distance in view pixels between grid lines. If this is < our desired spacing, increment it until it's not.
    // This involves dividing by zoom: units * (pixels/unit) = pixels.
    //float pixelsPerGridSpace = (float)minGrid / zoom();
    while ( (float)minGrid / zoom() < minPixelSpacing() )
    {
        minGrid *= 2;
    }

    // Find the first and last grid lines within min and max.
    QPoint minOnGrid, maxOnGrid;
    lowerUpperGridBounds(min, max, minGrid, minOnGrid, maxOnGrid);

    QGeometryData data;
    int currentIndex = 0;

    for ( int i = minOnGrid.x(); i <= maxOnGrid.x(); i += minGrid )
    {
        QColor lineColour = colourForCoordinate(i);

        // Convert the world-space co-ordinate back into view space.
        // We are i units away from the world origin on x, but we need to know
        // how far away we are from the centre of the view.
        // The view centre is given in world units by gridCentre;
        // therefore, the world distance of i from gridCentre is:
        //float worldDistFromGridCentre = -center().x() + (float)i;

        // Convert this to the pixel distance in view space.
        //float pixelDistanceFromViewCentre = worldDistFromGridCentre / zoom();
        QVector2D viewPos = gridToViewFull(QVector2D(i, 0));

        // Calculate the actual pixel that this line lies on in the view.
        //float viewPixel = viewCentre.x() + pixelDistanceFromViewCentre;

        // Now convert this pixel value into an OpenGL screen co-ordinate,
        // from -1 to 1 (with 0 being central).
        //float screenCoord = -1.0f + ((viewPixel / (float)(m_Size.width()-1)) * 2.0f);
        QVector2D screenPos = viewToScreen(viewPos);

        // Add a line to the QGeometryData for this position.
        //data.appendVertex(QVector3D(screenCoord, minScreen.y(), 0), QVector3D(screenCoord, maxScreen.y(), 0));
        data.appendVertex(QVector3D(screenPos.x(), minScreen.y(), 0), QVector3D(screenPos.x(), maxScreen.y(), 0));
        data.appendColor(lineColour, lineColour);
        data.appendIndex(currentIndex++);
        data.appendIndex(currentIndex++);
    }

    for ( int j = minOnGrid.y(); j <= maxOnGrid.y(); j += minGrid )
    {
        QColor lineColour = colourForCoordinate(j);
        QVector2D viewPos = gridToViewFull(QVector2D(0, j));
        QVector2D screenPos = viewToScreen(viewPos);
        data.appendVertex(QVector3D(minScreen.x(), screenPos.y(), 0), QVector3D(maxScreen.x(), screenPos.y(), 0));
        data.appendColor(lineColour, lineColour);
        data.appendIndex(currentIndex++);
        data.appendIndex(currentIndex++);
    }

    return data;
}

QVector2D MapViewGrid::viewToScreen(const QVector2D &vec) const
{
    float w2 = (float)m_Size.width()/2.0f;
    float h2 = (float)m_Size.height()/2.0f;
    return QVector2D(
                Model_Util::remap(vec.x(), -w2, w2, -1, 1),
                Model_Util::remap(vec.y(), -h2, h2, -1, 1)
                );
}

QVector2D MapViewGrid::screenToView(const QVector2D &vec) const
{
    float w2 = (float)m_Size.width()/2.0f;
    float h2 = (float)m_Size.height()/2.0f;
    return QVector2D(
                Model_Util::remap(vec.x(), -1, 1, -w2, w2),
                Model_Util::remap(vec.y(), -1, 1, -h2, h2)
                );
}

void MapViewGrid::lowerUpperGridBounds(const QVector2D &min, const QVector2D &max, int gridSpacing,
                                       QPoint &lowerBound, QPoint &upperBound)
{
    float gs = (float)gridSpacing;
    float division, remainder;

    division = min.x() / gs;
    remainder = division - qFloor(division);
    if ( qFuzzyIsNull(remainder) ) lowerBound.setX((int)min.x());
    else lowerBound.setX((int)(min.x() + (gs - (remainder * gs))));

    division = min.y() / gs;
    remainder = division - qFloor(division);
    if ( qFuzzyIsNull(remainder) ) lowerBound.setY((int)min.y());
    else lowerBound.setY((int)(min.y() + (gs - (remainder * gs))));

    division = max.x() / gs;
    remainder = division - qFloor(division);
    if ( qFuzzyIsNull(remainder) ) upperBound.setX((int)max.x());
    else upperBound.setX((int)(max.x() - (remainder * gs)));

    division = max.y() / gs;
    remainder = division - qFloor(division);
    if ( qFuzzyIsNull(remainder) ) upperBound.setY((int)max.y());
    else upperBound.setY((int)(max.y() - (remainder * gs)));
}

void MapViewGrid::draw(QGLPainter *painter, const MapView2D *view)
{
    if ( view->size().isEmpty() ) return;

    bool depthTest = painter->glIsEnabled(GL_DEPTH_TEST);
    if ( depthTest ) painter->glDisable(GL_DEPTH_TEST);

    grid(view)->draw(painter);

    if ( depthTest ) painter->glEnable(GL_DEPTH_TEST);
}

QColor MapViewGrid::colourForCoordinate(int i) const
{
    // Priority:
    // If i == 0, return origin colour.
    // If i % 1024 == 0, return major colour.
    // If i % 64 == 0, return minor colour.
    // Else return standard colour.

    // We assume that this function is only called when a grid line colour is desired -
    // don't call for non-gridline numbers as it'll just return standardColor() all the time.

    if ( i == 0 ) return originColor();
    if ( (i % 1024) == 0 ) return majorColor();
    if ( (i % 64) == 0 ) return minorColor();

    return standardColor();
}

float MapViewGrid::minPixelSpacing() const
{
    return m_flMinPixelSpacing;
}

void MapViewGrid::setMinPixelSpacing(float spacing)
{
    if ( spacing == m_flMinPixelSpacing ) return;
    
    m_flMinPixelSpacing = spacing;
    if ( m_flMinPixelSpacing < 0.0f ) m_flMinPixelSpacing = 0.0f;
    emit minPixelSpacingChanged(m_flMinPixelSpacing);
}

QMatrix4x4 MapViewGrid::viewToGridTransform(const QVector3D &translation)
{
    return QMatrix4x4( 1, 0, 0, translation.x(),
                       0, 1, 0, translation.y(),
                       0, 0, 1, 0,
                       0, 0, 0, 1 );
}

QMatrix4x4 MapViewGrid::viewToGridTransform(float zoom)
{
    return QMatrix4x4( zoom, 0,    0, 0,
                       0,    zoom, 0, 0,
                       0,    0,    1, 0,
                       0,    0,    0, 1 );
}

QVector2D MapViewGrid::vec4ToVec2(const QVector4D &vec)
{
    return QVector2D(vec.x(), vec.y());
}

QVector4D MapViewGrid::vec2ToVec4(const QVector2D &vec)
{
    return QVector4D(vec.x(), vec.y(), 0, 1);
}

VIEW_END_NAMESPACE
