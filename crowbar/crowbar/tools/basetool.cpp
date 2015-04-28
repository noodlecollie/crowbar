#include "basetool.h"
#include <QPoint>
#include "mapview.h"
#include <QtDebug>
#include "mapdocument.h"
#include "model_util.h"
#include "mapview2d.h"

using namespace VIEW_NAMESPACE;
using namespace MODEL_NAMESPACE;

TOOLS_BEGIN_NAMESPACE

BaseTool::BaseTool(QObject *parent) :
    QObject(parent)
{
    m_pDocument = NULL;
    m_bActive = false;
    m_bInDrag = false;
    m_bInRightDrag = false;
    m_bInMouseOver = false;
    m_bSnapping = true;
    m_bGridCoords = false;
    m_bRightGridCoords = false;
}

BaseTool::~BaseTool()
{
}

bool BaseTool::isActive() const
{
    return m_bActive;
}

void BaseTool::activate()
{
    if ( m_pDocument.isNull() ) return;
    m_bActive = true;
    processActivate();
    emit activated();
}

void BaseTool::deactivate()
{
    if ( isDragActive() ) dragEnd();
    if ( isMouseOverActive() ) mouseOverEnd();
    processDeactivate();
    m_bActive = false;
    emit deactivated();
}

void BaseTool::updateGridValues(MapView2D *view)
{
    if ( !view )
    {
        m_vecGridBegin = QVector2D();
        m_vecGridCurrent = m_vecGridBegin;
        m_vecGridLast = m_vecGridCurrent;
        m_bGridCoords = false;
        return;
    }
    
    QMatrix4x4 clientToGrid = Model_Util::clientToGrid(view->zoom(), view->size(), view->position());

    m_vecGridBegin = Model_Util::multiply(clientToGrid, QVector2D(m_PointBegin));
    m_vecGridLast = Model_Util::multiply(clientToGrid, QVector2D(m_PointLast));
    m_vecGridCurrent = Model_Util::multiply(clientToGrid, QVector2D(m_PointCurrent));
    
    if ( snapping() )
    {
        m_vecGridBegin = Model_Util::snapPowerTwo(document()->gridPower(), m_vecGridBegin);
        m_vecGridLast = Model_Util::snapPowerTwo(document()->gridPower(), m_vecGridCurrent);
        m_vecGridCurrent = Model_Util::snapPowerTwo(document()->gridPower(), m_vecGridLast);
    }
    
    m_bGridCoords = true;
}

void BaseTool::updateRightGridValues(MapView2D *view)
{
    if ( !view )
    {
        m_vecRightGridBegin = QVector2D();
        m_vecRightGridCurrent = m_vecRightGridBegin;
        m_vecRightGridLast = m_vecRightGridCurrent;
        m_bRightGridCoords = false;
        return;
    }
    
    QMatrix4x4 clientToGrid = Model_Util::clientToGrid(view->zoom(), view->size(), view->position());

    m_vecRightGridBegin = Model_Util::multiply(clientToGrid, QVector2D(m_RightPointBegin));
    m_vecRightGridLast = Model_Util::multiply(clientToGrid, QVector2D(m_RightPointLast));
    m_vecRightGridCurrent = Model_Util::multiply(clientToGrid, QVector2D(m_RightPointCurrent));
    
    if ( snapping() )
    {
        m_vecRightGridBegin = Model_Util::snapPowerTwo(document()->gridPower(), m_vecRightGridBegin);
        m_vecRightGridLast = Model_Util::snapPowerTwo(document()->gridPower(), m_vecRightGridCurrent);
        m_vecRightGridCurrent = Model_Util::snapPowerTwo(document()->gridPower(), m_vecRightGridLast);
    }
    
    m_bRightGridCoords = true;
}

void BaseTool::updateCurrentGridValues(MapView2D *view)
{
    if ( !view )
    {
        m_bGridCoords = false;
        return;
    }

    QMatrix4x4 clientToGrid = Model_Util::clientToGrid(view->zoom(), view->size(), view->position());
    m_vecGridLast = Model_Util::multiply(clientToGrid, QVector2D(m_PointLast));
    m_vecGridCurrent = Model_Util::multiply(clientToGrid, QVector2D(m_PointCurrent));

    if ( snapping() )
    {
        m_vecGridLast = Model_Util::snapPowerTwo(document()->gridPower(), m_vecGridCurrent);
        m_vecGridCurrent = Model_Util::snapPowerTwo(document()->gridPower(), m_vecGridLast);
    }

    m_bGridCoords = true;
}

void BaseTool::updateCurrentRightGridValues(MapView2D *view)
{
    if ( !view )
    {
        m_bRightGridCoords = false;
        return;
    }

    QMatrix4x4 clientToGrid = Model_Util::clientToGrid(view->zoom(), view->size(), view->position());
    m_vecRightGridLast = Model_Util::multiply(clientToGrid, QVector2D(m_RightPointLast));
    m_vecRightGridCurrent = Model_Util::multiply(clientToGrid, QVector2D(m_RightPointCurrent));

    if ( snapping() )
    {
        m_vecRightGridLast = Model_Util::snapPowerTwo(document()->gridPower(), m_vecRightGridCurrent);
        m_vecRightGridCurrent = Model_Util::snapPowerTwo(document()->gridPower(), m_vecRightGridLast);
    }

    m_bRightGridCoords = true;
}

bool BaseTool::gridCoordsValid() const
{
    return m_bGridCoords;
}

bool BaseTool::rightGridCoordsValid() const
{
    return m_bRightGridCoords;
}

QVector2D BaseTool::gridDragBegin() const
{
    return m_vecGridBegin;
}

QVector2D BaseTool::gridDragCurrent() const
{
    return m_vecGridCurrent;
}

QVector2D BaseTool::gridDragLast() const
{
    return m_vecGridLast;
}

QVector2D BaseTool::rightGridDragBegin() const
{
    return m_vecRightGridBegin;
}

QVector2D BaseTool::rightGridDragCurrent() const
{
    return m_vecRightGridCurrent;
}

QVector2D BaseTool::rightGridDragLast() const
{
    return m_vecRightGridLast;
}

void BaseTool::dragBegin(const QPoint &pos)
{
    if ( !hasActiveView() || !isActive() || isDragActive() ) return;
    
    m_bInDrag = true;
    m_PointBegin = pos;
    m_PointCurrent = m_PointBegin;
    m_PointLast = m_PointCurrent;
    
    updateGridValues(qobject_cast<MapView2D*>(activeView()));

    processDragBegin(pos);
}

void BaseTool::rightDragBegin(const QPoint &pos)
{
    if ( !hasActiveView() || !isActive() || isRightDragActive() ) return;
    
    m_bInRightDrag = true;
    m_RightPointBegin = pos;
    m_RightPointCurrent = m_RightPointBegin;
    m_RightPointLast = m_RightPointCurrent;
    
    updateGridValues(qobject_cast<MapView2D*>(activeView()));

    processRightDragBegin(pos);
}

void BaseTool::processDragBegin(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void BaseTool::processRightDragBegin(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void BaseTool::dragMove(const QPoint &pos)
{
    if ( !hasActiveView() || !isActive() || !isDragActive() ) return;
    
    m_PointLast = m_PointCurrent;
    m_PointCurrent = pos;
    
    updateCurrentGridValues(qobject_cast<MapView2D*>(activeView()));
    
    processDragMove(pos);
}

void BaseTool::rightDragMove(const QPoint &pos)
{
    if ( !hasActiveView() || !isActive() || !isRightDragActive() ) return;
    
    m_RightPointLast = m_RightPointCurrent;
    m_RightPointCurrent = pos;
    
    updateCurrentRightGridValues(qobject_cast<MapView2D*>(activeView()));
    
    processRightDragMove(pos);
}

void BaseTool::processDragMove(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void BaseTool::processRightDragMove(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void BaseTool::dragEnd()
{
    if ( !hasActiveView() || !isActive() || !isDragActive() ) return;
    
    processDragEnd();

    m_PointBegin = QPoint();
    m_PointCurrent = QPoint();
    m_PointLast = QPoint();
    m_vecGridBegin = QVector2D();
    m_vecGridCurrent = QVector2D();
    m_vecGridLast = QVector2D();
    
    m_bInDrag = false;
}

void BaseTool::rightDragEnd()
{
    if ( !hasActiveView() || !isActive() || !isRightDragActive() ) return;
    
    processRightDragEnd();

    m_RightPointBegin = QPoint();
    m_RightPointCurrent = QPoint();
    m_RightPointLast = QPoint();
    m_vecRightGridBegin = QVector2D();
    m_vecRightGridCurrent = QVector2D();
    m_vecRightGridLast = QVector2D();
    
    m_bInRightDrag = false;
}

void BaseTool::processDragEnd()
{
}

void BaseTool::processRightDragEnd()
{
}

void BaseTool::mouseOverBegin(const QPoint &pos)
{
    if ( !hasActiveView() || !isActive() || isMouseOverActive() ) return;

    m_bInMouseOver = true;
    processMouseOverBegin(pos);
}

void BaseTool::processMouseOverBegin(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void BaseTool::mouseOverMove(const QPoint &pos)
{
    if ( !hasActiveView() || !isActive() || !isMouseOverActive() ) return;

    processMouseOverMove(pos);
}

void BaseTool::processMouseOverMove(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void BaseTool::mouseOverEnd()
{
    if ( !hasActiveView() || !isActive() || !isMouseOverActive() ) return;
    
    processMouseOverEnd();
    m_bInMouseOver = false;
}

void BaseTool::processMouseOverEnd()
{
}

QPoint BaseTool::dragBegin() const
{
    return m_PointBegin;
}

QPoint BaseTool::dragCurrent() const
{
    return m_PointCurrent;
}

QPoint BaseTool::dragLast() const
{
    return m_PointLast;
}

QPoint BaseTool::rightDragBegin() const
{
    return m_RightPointBegin;
}

QPoint BaseTool::rightDragCurrent() const
{
    return m_RightPointCurrent;
}

QPoint BaseTool::rightDragLast() const
{
    return m_RightPointLast;
}

bool BaseTool::isDragActive() const
{
    return m_bInDrag;
}

bool BaseTool::isRightDragActive() const
{
    return m_bInRightDrag;
}

MapView* BaseTool::activeView() const
{
    return m_pView.data();
}

void BaseTool::setActiveView(MapView *view)
{
    if ( view == m_pView.data() ) return;
    
    if ( isDragActive() ) dragEnd();
    if ( isMouseOverActive() ) mouseOverEnd();
    
    manageViewConnections(m_pView.data(), view);
    m_pView = view;
    emit activeViewChanged(m_pView.data());
}

bool BaseTool::hasActiveView() const
{
    return !m_pView.isNull();
}

bool BaseTool::isMouseOverActive() const
{
    return m_bInMouseOver;
}

MapDocument* BaseTool::document() const
{
    return m_pDocument.data();
}

void BaseTool::setDocument(MapDocument *document)
{
    if ( m_pDocument.data() == document ) return;

    m_pDocument = document;
    emit documentChanged(m_pDocument.data());
}

void BaseTool::manageViewConnections(MapView *vOld, MapView *vNew)
{
    if ( vOld )
    {
        disconnect(vOld, SIGNAL(dragBegin(QPoint)), this, SLOT(dragBegin(QPoint)));
        disconnect(vOld, SIGNAL(dragMove(QPoint)), this, SLOT(dragMove(QPoint)));
        disconnect(vOld, SIGNAL(dragEnd()), this, SLOT(dragEnd()));
        
        disconnect(vOld, SIGNAL(rightDragBegin(QPoint)), this, SLOT(rightDragBegin(QPoint)));
        disconnect(vOld, SIGNAL(rightDragMove(QPoint)), this, SLOT(rightDragMove(QPoint)));
        disconnect(vOld, SIGNAL(rightDragEnd()), this, SLOT(rightDragEnd()));
        
        disconnect(vOld, SIGNAL(mouseOverBegin(QPoint)), this, SLOT(mouseOverBegin(QPoint)));
        disconnect(vOld, SIGNAL(mouseOverMove(QPoint)), this, SLOT(mouseOverMove(QPoint)));
        disconnect(vOld, SIGNAL(mouseOverEnd()), this, SLOT(mouseOverEnd()));

        disconnect(vOld, SIGNAL(viewParamsChanged()), this, SLOT(viewUpdate()));
    }

    if ( vNew )
    {
        connect(vNew, SIGNAL(dragBegin(QPoint)), this, SLOT(dragBegin(QPoint)));
        connect(vNew, SIGNAL(dragMove(QPoint)), this, SLOT(dragMove(QPoint)));
        connect(vNew, SIGNAL(dragEnd()), this, SLOT(dragEnd()));
        
        connect(vNew, SIGNAL(rightDragBegin(QPoint)), this, SLOT(rightDragBegin(QPoint)));
        connect(vNew, SIGNAL(rightDragMove(QPoint)), this, SLOT(rightDragMove(QPoint)));
        connect(vNew, SIGNAL(rightDragEnd()), this, SLOT(rightDragEnd()));
        
        connect(vNew, SIGNAL(mouseOverBegin(QPoint)), this, SLOT(mouseOverBegin(QPoint)));
        connect(vNew, SIGNAL(mouseOverMove(QPoint)), this, SLOT(mouseOverMove(QPoint)));
        connect(vNew, SIGNAL(mouseOverEnd()), this, SLOT(mouseOverEnd()));

        connect(vNew, SIGNAL(viewParamsChanged()), this, SLOT(viewUpdate()));
    }
}

bool BaseTool::snapping() const
{
    return m_bSnapping;
}

void BaseTool::setSnapping(bool enabled)
{
    if ( enabled == m_bSnapping ) return;

    m_bSnapping = enabled;
    emit snappingChanged(m_bSnapping);
}

void BaseTool::processExecute()
{
}

void BaseTool::execute()
{
    processExecute();
}

void BaseTool::viewUpdate()
{
    if ( !hasActiveView() || !isActive() || !isDragActive() ) return;

    processDragMove(activeView()->mapFromGlobal(QCursor::pos()));
}

void BaseTool::processActivate()
{
}

void BaseTool::processDeactivate()
{
}

TOOLS_END_NAMESPACE
