#include "toolcreatebrush.h"
#include "mapdocument.h"
#include <QtDebug>
#include "model_util.h"
#include <QtMath>
#include "effects/selectionboxeffect.h"
#include "mapbrushbuilder.h"
#include "mapbrush.h"

using namespace VIEW_NAMESPACE;
using namespace MODEL_NAMESPACE;

TOOLS_BEGIN_NAMESPACE

ToolCreateBrush::ToolCreateBrush(QObject *parent) :
    BaseTool(parent)
{
    m_bPickedHandle = false;
    m_bCentreHandle = false;
    m_bHandleTranslateOnX = false;
    
    connect(this, &BaseTool::activeViewChanged, this, &ToolCreateBrush::handleActiveViewChanged);
}

ToolCreateBrush::~ToolCreateBrush()
{
}

void ToolCreateBrush::setBounds(const QVector3D &v1, const QVector3D &v2)
{
    m_vecMin = QVector3D(qMin<float>(v1.x(), v2.x()), qMin<float>(v1.y(), v2.y()), qMin<float>(v1.z(), v2.z()));
    m_vecMax = QVector3D(qMax<float>(v1.x(), v2.x()), qMax<float>(v1.y(), v2.y()), qMax<float>(v1.z(), v2.z()));
}

void ToolCreateBrush::processDragBegin(const QPoint &pos)
{
    BaseTool::processDragBegin(pos);
    
    // TODO: In the future we'll want to maybe raycast here to get the intersection
    // point with a primitive's surface if we're creating within the 3D view.
    // For now, we'll just exit if the active view is not a 2D view.
    MapView2D* view = qobject_cast<MapView2D*>(activeView());
    if ( !view )
    {
        setBounds(QVector3D(), QVector3D());
        visualEffectsForDragBegin(pos);
        return;
    }
    
    // See if we picked a selection handle.
    QString handle;
    QVector3D anchor;
    QVector3D basePos;
    bool pickedHandle = activeView()->pickSelectionHandle(dragBegin(), handle, anchor, basePos);
    if ( pickedHandle )
    {
        // Get the handle's position on the grid.
        QMatrix4x4 viewToGrid = Model_Util::viewToGrid(view->zoom(), view->size(), view->position());
        m_vecHandleBaseGridPos = viewToGrid * basePos;
        m_vecHandleAnchorGridPos = viewToGrid * anchor;
        m_bPickedHandle = true;
        m_CachedBox.setExtents(m_vecWorldMin, m_vecWorldMax);

        Q_ASSERT(handle.length() == 2);
        if ( handle.at(0) == 'm' )
        {
            m_bCentreHandle = true;
            m_bHandleTranslateOnX = false;
        }
        else if ( handle.at(1) == 'm' )
        {
            m_bCentreHandle = true;
            m_bHandleTranslateOnX = true;
        }
        else
        {
            m_bCentreHandle = false;
        }
        
        visualEffectsForDragBegin(pos);
        return;
    }
    
    // Get the bounding box of the current selection.
    QBox3D bbox = document()->selectedSet().globalBoundingBox();
    float min = -64, max = 0;
    
    // If the box is valid, use it to calculate our depth in grid space.
    // Otherwise, just leave as 0 to -64.
    if ( bbox.isFinite() && !bbox.isNull() )
    {
        QMatrix4x4 worldToGrid = view->gridToWorld(view->direction()).inverted();
        QVector3D vmin = worldToGrid * bbox.minimum();
        QVector3D vmax = worldToGrid * bbox.maximum();
        min = vmin.z();
        max = vmax.z();
    }

    // Set our bounds.
    QVector2D cursorOnGrid = gridDragBegin();
    QVector3D resultA(cursorOnGrid.x(), cursorOnGrid.y(), min);
    QVector3D resultB = resultA;
    resultB.setZ(max);
    setBounds(resultA, resultB);

    visualEffectsForDragBegin(pos);
}

void ToolCreateBrush::visualEffectsForDragBegin(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void ToolCreateBrush::processDragMove(const QPoint &pos)
{
    BaseTool::processDragMove(pos);

    MapView2D* view = qobject_cast<MapView2D*>(activeView());
    if ( !view )
    {
        setBounds(QVector3D(), QVector3D());
        visualEffectsForDragMove(pos);
        return;
    }
    
    if ( m_bPickedHandle )
    {
        QMatrix4x4 worldToGrid = view->gridToWorld().inverted();
        float min = (worldToGrid * m_CachedBox.minimum()).z();
        float max = (worldToGrid * m_CachedBox.maximum()).z();

        QVector3D v1 = m_vecHandleAnchorGridPos;
        QVector3D v2 = gridDragCurrent();

        if ( m_bCentreHandle )
        {
            if ( m_bHandleTranslateOnX )
            {
                v1.setY((worldToGrid * m_CachedBox.minimum()).y());
                v2.setY((worldToGrid * m_CachedBox.maximum()).y());
            }
            else
            {
                v1.setX((worldToGrid * m_CachedBox.minimum()).x());
                v2.setX((worldToGrid * m_CachedBox.maximum()).x());
            }
        }

        v1.setZ(min);
        v2.setZ(max);

        setBounds(v1,v2);
        
        visualEffectsForDragMove(pos);
        return;
    }

    // Get our current depth.
    float min = m_vecMin.z();
    float max = m_vecMax.z();

    // Set the bounds appropriately.
    QVector2D begin = gridDragBegin();
    QVector2D current = gridDragCurrent();
    setBounds(QVector3D(begin.x(), begin.y(), min), QVector3D(current.x(), current.y(), max));

    visualEffectsForDragMove(pos);
}

void ToolCreateBrush::visualEffectsForDragMove(const QPoint &pos)
{
    Q_UNUSED(pos);

    MapView2D* view = qobject_cast<MapView2D*>(activeView());
    if ( !view ) return;

    if ( m_pBox.isNull() ) m_pBox = new ToolWireframeBox(document()->world());

    QMatrix4x4 gridToWorld = view->gridToWorld();
    QVector3D min = gridToWorld * m_vecMin;
    QVector3D max = gridToWorld * m_vecMax;
    QBox3D b(min, max);
    m_pBox->setBox(b);
    view->setSelectionBounds(b);
}

void ToolCreateBrush::processDragEnd()
{
    BaseTool::processDragEnd();
    
    m_vecHandleBaseGridPos = QVector3D();
    m_vecHandleAnchorGridPos = QVector3D();
    m_CachedBox = QBox3D();
    m_bPickedHandle = false;
    m_bCentreHandle = false;
    m_bHandleTranslateOnX = false;

    MapView2D* view = qobject_cast<MapView2D*>(activeView());
    if ( view )
    {
        QMatrix4x4 gridToWorld = view->gridToWorld();
        m_vecWorldMin = gridToWorld * m_vecMin;
        m_vecWorldMax = gridToWorld * m_vecMax;
    }

    QBox3D b(m_vecMin, m_vecMax);
    if ( b.isNull() || b.isInfinite() )
    {
        setBounds(QVector3D(), QVector3D());
        activeView()->setSelectionBounds(QBox3D());
        delete m_pBox.data();
        return;
    }

    visualEffectsForDragEnd();
}

void ToolCreateBrush::visualEffectsForDragEnd()
{
    MapView2D* view = qobject_cast<MapView2D*>(activeView());
    if ( !view ) return;

    if ( m_pBox.isNull() ) m_pBox = new ToolWireframeBox(document()->world());

    QMatrix4x4 gridToWorld = view->gridToWorld();
    QVector3D min = gridToWorld * m_vecMin;
    QVector3D max = gridToWorld * m_vecMax;
    QBox3D b(min, max);
    m_pBox->setBox(b);
    view->setSelectionBounds(b);
}

QString ToolCreateBrush::toolName() const
{
    return QString("Create Brush");
}

void ToolCreateBrush::processExecute()
{
    BaseTool::processExecute();
    
    MapView2D* view = qobject_cast<MapView2D*>(activeView());
    if ( !view )
    {
        return;
    }
    
    // For now we just assume that the grid is aligned with some world axes.
    QMatrix4x4 gridToWorld = view->gridToWorld();
    QVector3D min = gridToWorld * m_vecMin;
    QVector3D max = gridToWorld * m_vecMax;
    
    QBox3D b(min,max);  // Setting this ensures the min and max are computed correctly, so we don't get inside-out brushes.
    if ( b.isInfinite() || Model_Util::hasZeroComponent(m_vecMin, m_vecMax) )
    {
        setBounds(QVector3D(), QVector3D());
        delete m_pBox.data();
        return;
    }
    
    qDebug() << "Selection bounds:" << document()->selectedSet().globalBoundingBox();
    
    MapBrush* brush = MapBrushBuilder::axisAlignedBrush(/*b.minimum(), b.maximum()*/b, document()->currentTextureImage());
    brush->setParentComponent(document()->world());
    document()->selectedSet().clear();
    document()->selectedSet().add(brush);
    
    setBounds(QVector3D(), QVector3D());
    delete m_pBox.data();
}

void ToolCreateBrush::processDeactivate()
{
    BaseTool::processDeactivate();
    
    setBounds(QVector3D(), QVector3D());
    delete m_pBox.data();
    
    for ( QSet<MapView*>::iterator it = m_EncounteredViews.begin(); it != m_EncounteredViews.end(); ++it )
    {
        MapView* v = *it;
        if ( v ) v->setSelectionBounds(QBox3D());
    }
}

void ToolCreateBrush::handleActiveViewChanged(MapView *view)
{
    if ( !isActive() || !view || !document() ) return;
    m_EncounteredViews.insert(QPointer<MapView>(view));
    view->setSelectionBounds(m_pBox.isNull() ? QBox3D() : m_pBox->box());
}

TOOLS_END_NAMESPACE
