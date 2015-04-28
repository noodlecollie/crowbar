#include "toolselect.h"
#include "mapview.h"
#include <QList>
#include "mapcomponent.h"
#include "mapdocumentview.h"
#include "mapdocument.h"
#include <QtDebug>
#include "model_util.h"
#include "mapview2d.h"
#include <QtMath>
#include "vieweffects.h"
#include "mapbrush.h"
#include "mapview2d.h"
#include <QPair>

using namespace MODEL_NAMESPACE;
using namespace VIEW_NAMESPACE;

TOOLS_BEGIN_NAMESPACE

ToolSelect::ToolSelect(QObject *parent):
    BaseTool(parent)
{
    m_iOperation = Replace;
    m_bShouldMoveDrag = false;
    m_bMouseOverCrosshair = false;
    m_bDragCrosshair = false;
    m_bDragBox = false;
    m_bHaveDragged = false;
    m_pOperator = NULL;

    connect(this, &BaseTool::activeViewChanged, this, &ToolSelect::handleActiveViewChanged);
}

ToolSelect::~ToolSelect()
{
}

ToolSelect::SelectionOperation ToolSelect::selectionOperation() const
{
    return m_iOperation;
}

void ToolSelect::setSelectionOperation(SelectionOperation operation)
{
    if ( operation == m_iOperation ) return;

    m_iOperation = operation;
    emit selectionOperationChanged(m_iOperation);
}

void ToolSelect::processDragBegin(const QPoint &pos)
{
    BaseTool::processDragBegin(pos);
    visualEffectsForDragBegin(pos);

    m_bHaveDragged = false;
    
    // Make sure we don't move-drag in the 3D view right now -
    // the micro-translations are more annoying than anything else.
    m_bShouldMoveDrag = false;
    MapView2D* v2d = qobject_cast<MapView2D*>(activeView());
    if ( v2d )
    {
        // If we select anything with our first click, don't drag-select -
        // translate the selected set instead.
        QPoint p = dragBegin();
        QRect r(p.x(), p.y(), 1, 1);
        QList<MapComponent*> list = activeView()->objectsWithin(r);
        if ( list.count() > 0 )
        {
            m_bShouldMoveDrag = true;
    
            // If not subtracting, do the operation with the object first.
            if ( m_iOperation != Subtract )
            {
                QList<MapComponent*> l;
                l.append(list.at(0));
                performOperation(l);
            }
    
            // Otherwise, just do a replace.
            else if ( !document()->selectedSet().contains(list.at(0)) )
            {
                document()->selectedSet().clear();
                document()->selectedSet().add(list.at(0));
            }
        }
        else if ( pointWithinSelectedSetBounds(p) )
        {
            m_bShouldMoveDrag = true;
        }
        else
        {
            QString handle;
            QVector3D anchor;
            QVector3D basePos;
            bool pickedHandle = activeView()->pickSelectionHandle(dragBegin(), handle, anchor, basePos);
            if ( !pickedHandle ) return;

            // Get the axes from the handle.
            QVector3D xAxis = MapView2D::gridXAxisInWorld(v2d->direction());
            QVector3D yAxis = MapView2D::gridYAxisInWorld(v2d->direction());
            
            Q_ASSERT(handle.length() == 2);
            switch ( handle.at(0).toLatin1() )
            {
                case '+':
                {
                    m_vecHandleTransX = xAxis;
                    break;
                }
                
                case '-':
                {
                    m_vecHandleTransX = -xAxis;
                    break;
                }
                
                default:
                {
                    m_vecHandleTransX = QVector3D();
                    break;
                }
            }
            
            switch ( handle.at(1).toLatin1() )
            {
                case '+':
                {
                    m_vecHandleTransY = yAxis;
                    break;
                }
                
                case '-':
                {
                    m_vecHandleTransY = -yAxis;
                    break;
                }
                
                default:
                {
                    m_vecHandleTransY = QVector3D();
                    break;
                }
            }

            QMatrix4x4 viewToWorld = v2d->gridToWorld() * Model_Util::viewToGrid(v2d->zoom(), v2d->size(), v2d->position());
            m_vecHandleAnchor = viewToWorld * anchor;
            m_vecHandleBasePos = viewToWorld * basePos;
            
            m_pOperator = new BrushVertexOperator(this);
            m_pOperator->setOrigin(m_vecHandleAnchor);
            
            for ( MapComponentSet::Set::iterator it = document()->selectedSet().begin();
                  it != document()->selectedSet().end();
                  ++it )
            {
                MapBrush* brush = qobject_cast<MapBrush*>(*it);
                if ( !brush ) continue;
                
                m_pOperator->addBrush(brush);
            }
        }

        v2d->setSelectionBounds(document()->selectedSet().globalBoundingBox());
    }
}

void ToolSelect::visualEffectsForDragBegin(const QPoint &pos)
{
    activeView()->setDrawRect(QRect());

    if ( dragCrosshairEnabled() && !m_bShouldMoveDrag ) activeView()->setCrosshair(dragCurrent(), QColor(Qt::white));
    else activeView()->clearCrosshair();

    m_CachedMouseOverPos = pos;
}

void ToolSelect::processDragMove(const QPoint &pos)
{
    BaseTool::processDragMove(pos);
    visualEffectsForDragMove(pos);

    m_bHaveDragged = true;
    
    const QGLCamera* cam = activeView()->camera();
    QMatrix4x4 mvmi = cam->modelViewMatrix().inverted();
    
    MapView2D* v2d = qobject_cast<MapView2D*>(activeView());
    if ( v2d )
    {
        if ( m_bShouldMoveDrag )
        {
            QPoint current = dragCurrent();
            QPoint begin;
            
            if ( snapping() )
            {
                QMatrix4x4 gridToClient = Model_Util::clientToGrid(v2d->zoom(), v2d->size(), v2d->position()).inverted();
                QVector2D temp = Model_Util::multiply(gridToClient, gridDragBegin());
                begin = QPoint((int)temp.x(), (int)temp.y());
            }
            else
            {
                begin = dragBegin();
            }
            
            float ratio = (float)activeView()->size().width()/(float)activeView()->size().height();
            QVector3D vCurrent = mvmi * cam->mapPoint(current, ratio, activeView()->size());
            QVector3D vBegin = mvmi * cam->mapPoint(begin, ratio, activeView()->size());
            QVector3D translation = (vCurrent - vBegin);
            
            QMatrix4x4 gridToWorld = v2d->gridToWorld(v2d->direction());
            QMatrix4x4 worldToGrid = gridToWorld.inverted();
            
            for ( MapComponentSet::Set::iterator it = document()->selectedSet().begin();
                  it != document()->selectedSet().end();
                  ++it )
            {
                MapComponent* c = (*it);
        
                // If the brush has a parent in the selected set, don't translate its origin.
                if ( document()->selectedSet().contains(c->parentComponent()) ) continue;
                
                if ( !snapping() )
                {
                    c->hierarchyTransform()->setTemporaryTranslation(translation);
                    continue;
                }
        
                // Calculate what the world position would be.
                QVector3D worldPos = c->hierarchyTransform()->translation() + translation;
                
                // Calculate the snapped position.
                // If we have the control modifier pressed, halve our grid power for snapping.
                int power = controlModifier() ? document()->gridPower() - 1 : document()->gridPower();
    
                QVector3D worldPosOnGrid = worldToGrid * worldPos;
                QVector3D snappedGridPos = Model_Util::snapPowerTwo(power, worldPosOnGrid);
                QVector3D snappedWorldPos = gridToWorld * snappedGridPos;
                
                // Calculate the new translation that we need.
                QVector3D newTrans = snappedWorldPos - c->hierarchyTransform()->translation();
                
                // Set this.
                c->hierarchyTransform()->setTemporaryTranslation(newTrans);
            }
        }
        else if ( !m_vecHandleTransX.isNull() || !m_vecHandleTransY.isNull() )
        {
            QPoint current = dragCurrent();
            QVector3D worldCurrent;

            if ( snapping() )
            {
                QVector2D g = gridDragCurrent();
                worldCurrent = v2d->gridToWorld() * QVector3D(g.x(), g.y(), 0);
            }
            else
            {
                worldCurrent = v2d->gridToWorld() * Model_Util::clientToGrid(v2d->zoom(), v2d->size(), v2d->position()) * QVector3D(current.x(), current.y(), 0);
            }
            
            // Calculate the distances d1 and d2.
            // d1 is the distance from the anchor to the handle base point;
            // d2 is the distance from the anchor to the current point.

            QVector3D anchorToBase = m_vecHandleBasePos - m_vecHandleAnchor;
            QVector3D anchorToCurrent = worldCurrent - m_vecHandleAnchor;

            float scale1Base = QVector3D::dotProduct(anchorToBase, m_vecHandleTransX);
            float scale1Current = QVector3D::dotProduct(anchorToCurrent, m_vecHandleTransX);
            float scale2Base = QVector3D::dotProduct(anchorToBase, m_vecHandleTransY);
            float scale2Current = QVector3D::dotProduct(anchorToCurrent, m_vecHandleTransY);

            QVector3D d1 = (scale1Base * m_vecHandleTransX) + (scale2Base * m_vecHandleTransY);
            QVector3D d2 = (scale1Current * m_vecHandleTransX) + (scale2Current * m_vecHandleTransY);
            
            // Calculate the scale values in X, Y and Z.
            // This is d2.x/d1.x etc.
            QVector3D scale(d1.x() == 0.0f ? 1.0f : d2.x()/d1.x(),
                            d1.y() == 0.0f ? 1.0f : d2.y()/d1.y(),
                            d1.z() == 0.0f ? 1.0f : d2.z()/d1.z());

            // TODO: Make this better. We really just want to flip the face orientations.
            if ( scale.x() < 0 ) scale.setX(-scale.x());
            if ( scale.y() < 0 ) scale.setY(-scale.y());
            if ( scale.z() < 0 ) scale.setZ(-scale.z());
            
            // Create a scale matrix for the operator.
            m_pOperator->setOperatorMatrix(Model_Util::scaleMatrix(scale));
            
            // Get the vertices back.
            QHash<MapBrush*, BrushVertexOperator::OperationResults > hash = m_pOperator->performOperation();
            
            for ( MapComponentSet::Set::iterator it = document()->selectedSet().begin();
                  it != document()->selectedSet().end();
                  ++it )
            {

                MapBrush* brush = qobject_cast<MapBrush*>(*it);
                if ( !brush ) continue;
                
                BrushVertexOperator::OperationResults res = hash.value(brush);
                QList<QVector3D> positions = res.vertices;
                QVector3D newOrigin = brush->hierarchyTransform()->globalTransformMatrix().inverted()
                        * brush->hierarchyTransform()->transformMatrix() * res.globalOriginPoition;
                Q_ASSERT(positions.count() == brush->vertices().count());
                
                for ( int i = 0; i < positions.count(); i++ )
                {
                    brush->vertices(i)->setPosition(positions.at(i));
                }

                brush->hierarchyTransform()->setTranslation(newOrigin);
            }
        }

        v2d->setSelectionBounds(document()->selectedSet().globalBoundingBox());
    }
//    else
//    {
//        QPoint current = dragCurrent();
//        QPoint begin = dragBegin();
//        float ratio = (float)activeView()->size().width()/(float)activeView()->size().height();
//        QVector3D vCurrent = mvmi * cam->mapPoint(current, ratio, activeView()->size());
//        QVector3D vBegin = mvmi * cam->mapPoint(begin, ratio, activeView()->size());
//        QVector3D translation = (vCurrent - vBegin);
        
//        for ( MapComponentSet::Set::iterator it = document()->selectedSet().begin();
//              it != document()->selectedSet().end();
//              ++it )
//        {
//            MapComponent* c = (*it);
    
//            // If the brush has a parent in the selected set, don't translate its origin.
//            if ( document()->selectedSet().contains(c->parentComponent()) ) continue;
    
//            c->hierarchyTransform()->setTemporaryTranslation(translation);
//        }
//    }
}

void ToolSelect::visualEffectsForDragMove(const QPoint &pos)
{
    Q_UNUSED(pos);
    QVector2D m_vecBegin;
    MapView2D* v2d = qobject_cast<MapView2D*>(activeView());
    if ( snapping() && v2d )
    {
        QMatrix4x4 gridToClient = Model_Util::clientToGrid(v2d->zoom(), v2d->size(), v2d->position()).inverted();
        m_vecBegin = Model_Util::multiply(gridToClient, gridDragBegin());
    }
    else
    {
        m_vecBegin = QVector2D(dragBegin());
    }
    QVector2D m_vecCurrent(dragCurrent());

    if ( m_vecBegin != m_vecCurrent && dragBoxEnabled() && !m_bShouldMoveDrag && m_vecHandleTransX.isNull() && m_vecHandleTransY.isNull() )
    {
        MapView2D* v2d = qobject_cast<MapView2D*>(activeView());
        if ( snapping() && v2d )
        {
            QVector2D vBegin = Model_Util::snapToGrid(m_vecBegin, document()->gridPower(), v2d->zoom(), v2d->size(), v2d->position());
            QVector2D vCurrent = Model_Util::snapToGrid(m_vecCurrent, document()->gridPower(), v2d->zoom(), v2d->size(), v2d->position());

            int left = qMin<int>(qFloor(vBegin.x()), qFloor(vCurrent.x()));
            int right = qMax<int>(qCeil(vBegin.x()), qCeil(vCurrent.x()));
            int top = qMin<int>(qFloor(vBegin.y()), qFloor(vCurrent.y()));
            int bottom = qMax<int>(qCeil(vBegin.y()), qCeil(vCurrent.y()));

            // Rect needs to be +1 on width and height, because a rect with a beginning
            // and current pos that are the same counts as a 1x1 rect.
            QRect rect(left, top, right - left + 1, bottom - top + 1);
            v2d->setDrawRect(rect, QColor(Qt::white), Effects::presetEffect(Effects::SelectionBox));
        }
        else
        {
            int left = qMin<int>(qFloor(m_vecBegin.x()), qFloor(m_vecCurrent.x()));
            int right = qMax<int>(qCeil(m_vecBegin.x()), qCeil(m_vecCurrent.x()));
            int top = qMin<int>(qFloor(m_vecBegin.y()), qFloor(m_vecCurrent.y()));
            int bottom = qMax<int>(qCeil(m_vecBegin.y()), qCeil(m_vecCurrent.y()));

            // Rect needs to be +1 on width and height, because a rect with a beginning
            // and current pos that are the same counts as a 1x1 rect.
            QRect rect(left, top, right - left + 1, bottom - top + 1);

            // TODO: Allow subclasses to set the rectangle effect.
            activeView()->setDrawRect(rect, QColor(Qt::white), Effects::presetEffect(Effects::SelectionBox));
        }
    }
    else
    {
        activeView()->setDrawRect(QRect());
    }

    if ( dragCrosshairEnabled() ) activeView()->setCrosshair(QPoint((int)m_vecCurrent.x(), (int)m_vecCurrent.y()), QColor(Qt::white));
    else activeView()->clearCrosshair();
}

void ToolSelect::processDragEnd()
{
    BaseTool::processDragEnd();
    visualEffectsForDragEnd();

    m_vecHandleAnchor = QVector3D();
    m_vecHandleBasePos = QVector3D();
    delete m_pOperator;
    m_pOperator = NULL;

    if ( m_bShouldMoveDrag && m_bHaveDragged )
    {
        for ( MapComponentSet::Set::iterator it = document()->selectedSet().begin();
              it != document()->selectedSet().end();
              ++it )
        {
            MapComponent* c = (*it);
    
            // If the brush has a parent in the selected set, don't translate its origin.
            if ( document()->selectedSet().contains(c->parentComponent()) ) continue;
    
            c->hierarchyTransform()->mergeTemporaryTranslation();
        }
        
        activeView()->setSelectionBounds(document()->selectedSet().globalBoundingBox());
        m_bShouldMoveDrag = false;
        return;
    }
    else if ( !m_vecHandleTransX.isNull() || !m_vecHandleTransY.isNull() )
    {
        m_vecHandleTransX = QVector3D();
        m_vecHandleTransY = QVector3D();
        activeView()->setSelectionBounds(document()->selectedSet().globalBoundingBox());
        return;
    }

    QVector2D m_vecBegin;
    MapView2D* v2d = qobject_cast<MapView2D*>(activeView());
    if ( snapping() && v2d )
    {
        QMatrix4x4 gridToClient = Model_Util::clientToGrid(v2d->zoom(), v2d->size(), v2d->position()).inverted();
        m_vecBegin = Model_Util::multiply(gridToClient, gridDragBegin());
    }
    else
    {
        m_vecBegin = QVector2D(dragBegin());
    }
    QVector2D m_vecCurrent(dragCurrent());
    int left = qMin<int>(qFloor(m_vecBegin.x()), qFloor(m_vecCurrent.x()));
    int right = qMax<int>(qCeil(m_vecBegin.x()), qCeil(m_vecCurrent.x()));
    int top = qMin<int>(qFloor(m_vecBegin.y()), qFloor(m_vecCurrent.y()));
    int bottom = qMax<int>(qCeil(m_vecBegin.y()), qCeil(m_vecCurrent.y()));
    QRect rect(left, top, right - left + 1, bottom - top + 1);

    performOperation(activeView()->objectsWithin(rect));
    activeView()->setSelectionBounds(document()->selectedSet().globalBoundingBox());
}

void ToolSelect::visualEffectsForDragEnd()
{
    activeView()->setDrawRect(QRect());
    activeView()->clearCrosshair();
    if ( isMouseOverActive() ) mouseOverMove(m_CachedMouseOverPos);
}

void ToolSelect::processMouseOverBegin(const QPoint &pos)
{
    visualEffectsForMouseOverBegin(pos);
}

void ToolSelect::processMouseOverMove(const QPoint &pos)
{
    visualEffectsForMouseOverMove(pos);
}

void ToolSelect::processMouseOverEnd()
{
    visualEffectsForMouseOverEnd();
}

void ToolSelect::visualEffectsForMouseOverBegin(const QPoint &pos)
{
    if ( mouseOverCrosshairEnabled() ) activeView()->setCrosshair(pos);
    else activeView()->clearCrosshair();
}

void ToolSelect::visualEffectsForMouseOverMove(const QPoint &pos)
{
    if ( isDragActive() )
    {
        m_CachedMouseOverPos = pos;
        return;
    }

    if ( mouseOverCrosshairEnabled() ) activeView()->setCrosshair(pos);
    else activeView()->clearCrosshair();
}

void ToolSelect::visualEffectsForMouseOverEnd()
{
    activeView()->clearCrosshair();
}

void ToolSelect::performOperation(const QList<MapComponent *> &list)
{
    if ( !document() ) return;

    if ( m_iOperation == Replace )
    {
        document()->selectedSet().clear();
        foreach ( MapComponent* c, list )
        {
            document()->selectedSet().add(c);
        }
        return;
    }

    if ( m_iOperation == Union || controlModifier() )
    {
        foreach ( MapComponent* c, list )
        {
            document()->selectedSet().add(c);
        }
        return;
    }

    if ( m_iOperation == Subtract )
    {
        foreach ( MapComponent* c, list )
        {
            document()->selectedSet().remove(c);
        }
        return;
    }
}

bool ToolSelect::dragBoxEnabled() const
{
    return m_bDragBox;
}

void ToolSelect::setDragBoxEnabled(bool enabled)
{
    if ( enabled == m_bDragBox ) return;

    m_bDragBox = enabled;
    emit dragBoxStateChanged(m_bDragBox);
}

bool ToolSelect::dragCrosshairEnabled() const
{
    return m_bDragCrosshair;
}

void ToolSelect::setDragCrosshairEnabled(bool enabled)
{
    if ( enabled == m_bDragCrosshair ) return;

    m_bDragCrosshair = enabled;
    emit dragCrosshairStateChanged(m_bDragCrosshair);
}

bool ToolSelect::mouseOverCrosshairEnabled() const
{
    return m_bMouseOverCrosshair;
}

void ToolSelect::setMouseOverCrosshairEnabled(bool enabled)
{
    if ( enabled == m_bMouseOverCrosshair ) return;

    m_bMouseOverCrosshair = enabled;
    emit mouseOverCrosshairStateChanged(m_bMouseOverCrosshair);
}

QString ToolSelect::toolName() const
{
    return QString("Select");
}

void ToolSelect::commandDelete(bool keyDown)
{
    if ( !keyDown || !isActive() || !document() ) return;

    QList<MapComponent*> components = document()->selectedSet().constData().toList();
    foreach ( MapComponent* c, components )
    {
        delete c;
    }

    for ( QSet<MapView*>::iterator it = m_EncounteredViews.begin(); it != m_EncounteredViews.end(); ++it )
    {
        MapView* v = *it;
        if ( v ) v->setSelectionBounds(QBox3D());
    }
}

bool ToolSelect::controlModifier() const
{
    return m_bControlModifier;
}

void ToolSelect::setControlModifier(bool enabled)
{
    if ( m_bControlModifier == enabled ) return;

    m_bControlModifier = enabled;
    emit controlModifierChanged(m_bControlModifier);
}

void ToolSelect::processActivate()
{
    BaseTool::processActivate();
}

void ToolSelect::processDeactivate()
{
    BaseTool::processDeactivate();
    for ( QSet<MapView*>::iterator it = m_EncounteredViews.begin(); it != m_EncounteredViews.end(); ++it )
    {
        MapView* v = *it;
        if ( v ) v->setSelectionBounds(QBox3D());
    }
}

void ToolSelect::handleActiveViewChanged(MapView *view)
{
    if ( !isActive() || !view || !document() ) return;
    m_EncounteredViews.insert(QPointer<MapView>(view));
    view->setSelectionBounds(document()->selectedSet().globalBoundingBox());
}

bool ToolSelect::pointWithinSelectedSetBounds(const QPoint &pos)
{
    if ( !document() ) return false;
    if ( !activeView() ) return false;

    QBox3D bounds = document()->selectedSet().globalBoundingBox();
    if ( bounds.isNull() ) return false;
    if ( bounds.isInfinite() ) return true;

    QGLCamera* cam = activeView()->camera();
    float aspectRatio = (float)activeView()->size().width() / (float)activeView()->size().height();
    QMatrix4x4 mat = Model_Util::clientToView(activeView()->size()).inverted() *
            cam->projectionMatrix(aspectRatio) * cam->modelViewMatrix();
    QBox3D clientBounds(mat * bounds.minimum(), mat * bounds.maximum());

    QVector3D min = clientBounds.minimum();
    QVector3D max = clientBounds.maximum();

    return pos.x() >= min.x() && pos.x() <= max.x() && pos.y() >= min.y() && pos.y() <= max.y();

}

void ToolSelect::processRightDragBegin(const QPoint &pos)
{
    BaseTool::processRightDragBegin(pos);
    
    // See if anything was selected.
    QPoint p = rightDragBegin();
    QRect r(p.x(), p.y(), 1, 1);
    QList<MapComponent*> list = activeView()->objectsWithin(r);
    MapComponent* parent = list.count() > 0 ? list.at(0) : document()->world();
    QMatrix4x4 worldToParent = (parent == document()->world()) ? QMatrix4x4() : parent->hierarchyTransform()->globalTransformMatrix().inverted();
    QList<QPair<MapComponent*, QMatrix4x4> > cachedMatrices;
    
    // Go through each item and re-parent.
    for ( MapComponentSet::Set::iterator it = document()->selectedSet().begin();
          it != document()->selectedSet().end();
          ++it )
    {
        MapComponent* c = (*it);
        
        // If the parent is the same as the current component, don't do anything.
        if ( c == parent ) continue;
        
        // Cache the matrices.
        cachedMatrices.append(QPair<MapComponent*, QMatrix4x4>(c, c->hierarchyTransform()->globalTransformMatrix() * worldToParent));
    }
    
    // Go through and apply each.
    // This ensures that all of the selected items stay in their original positions,
    // as if their parent is modified before them it could cause issues.
    // TODO: Is this right? I'll leave this here to be safe.
    for ( int i = 0; i < cachedMatrices.count(); i++ )
    {
        MapComponent* c = cachedMatrices.at(i).first;
        QVector3D translation;
        EulerAngle rotation;
        QVector3D scale;
        
        Model_Util::transformOrientation(cachedMatrices.at(i).second, translation, rotation, scale);
        
        qDebug() << "Component translation:" << c->hierarchyTransform()->translation() << "Component rotation:" << c->hierarchyTransform()->rotation()
                 << "Component scale:" << c->hierarchyTransform()->scale() << "\n"
                 << "Transform to parent space:" << c->hierarchyTransform()->transformMatrix() << "\n"
                 << "Transform to global space:" << c->hierarchyTransform()->globalTransformMatrix();
        
        // Set the parent.
        c->setParentComponent(parent);

        // Force an update on the parent.
        parent->forceRenderDataUpdate();
        
        // Set the attributes.
        c->hierarchyTransform()->setTranslation(translation);
        c->hierarchyTransform()->setRotation(rotation);
        c->hierarchyTransform()->setScale(scale);
    }
}

TOOLS_END_NAMESPACE
