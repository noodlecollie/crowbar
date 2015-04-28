#ifndef BASETOOL_H
#define BASETOOL_H

#include <QObject>
#include "tools_global.h"
#include "view_global.h"
#include "model_global.h"
#include <QPoint>
#include <QPointer>
#include <QVector2D>

MODEL_BEGIN_NAMESPACE
class MapDocument;
MODEL_END_NAMESPACE

VIEW_BEGIN_NAMESPACE
class MapView;
class MapView2D;
VIEW_END_NAMESPACE

class QGLAbstractEffect;

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT BaseTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(VIEW_NAMESPACE::MapView* activeView READ activeView WRITE setActiveView NOTIFY activeViewChanged)
    Q_PROPERTY(MODEL_NAMESPACE::MapDocument* document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(QString toolName READ toolName)
    Q_PROPERTY(bool snapping READ snapping WRITE setSnapping NOTIFY snappingChanged)
public:
    explicit BaseTool(QObject *parent = 0);
    virtual ~BaseTool();
    
    bool isActive() const;
    bool isDragActive() const;
    bool isRightDragActive() const;
    bool isMouseOverActive() const;
    
    VIEW_NAMESPACE::MapView* activeView() const;
    void setActiveView(VIEW_NAMESPACE::MapView* view);
    bool hasActiveView() const;

    MODEL_NAMESPACE::MapDocument* document() const;
    void setDocument(MODEL_NAMESPACE::MapDocument* document);

    virtual QString toolName() const = 0;

    bool snapping() const;
    void setSnapping(bool enabled);
    
    void execute();
    
signals:
    void activeViewChanged(VIEW_NAMESPACE::MapView*);
    void documentChanged(MODEL_NAMESPACE::MapDocument*);
    void activated();
    void deactivated();
    void snappingChanged(bool);
    
public slots:
    void activate();
    void deactivate();

    void dragBegin(const QPoint &pos);
    void dragMove(const QPoint &pos);
    void dragEnd();
    
    void rightDragBegin(const QPoint &pos);
    void rightDragMove(const QPoint &pos);
    void rightDragEnd();

    void mouseOverBegin(const QPoint &pos);
    void mouseOverMove(const QPoint &pos);
    void mouseOverEnd();

private slots:
    void viewUpdate();
    
protected:
    QPoint dragBegin() const;
    QPoint dragCurrent() const;
    QPoint dragLast() const;
    
    QPoint rightDragBegin() const;
    QPoint rightDragCurrent() const;
    QPoint rightDragLast() const;
    
    QVector2D gridDragBegin() const;
    QVector2D gridDragCurrent() const;
    QVector2D gridDragLast() const;
    
    QVector2D rightGridDragBegin() const;
    QVector2D rightGridDragCurrent() const;
    QVector2D rightGridDragLast() const;
    
    bool gridCoordsValid() const;
    bool rightGridCoordsValid() const;

    virtual void processDragBegin(const QPoint &pos);
    virtual void processDragMove(const QPoint &pos);
    virtual void processDragEnd();
    
    virtual void processRightDragBegin(const QPoint &pos);
    virtual void processRightDragMove(const QPoint &pos);
    virtual void processRightDragEnd();

    virtual void processMouseOverBegin(const QPoint &pos);
    virtual void processMouseOverMove(const QPoint &pos);
    virtual void processMouseOverEnd();
    
    virtual void processExecute();
    virtual void processActivate();
    virtual void processDeactivate();
    
private:
    void manageViewConnections(VIEW_NAMESPACE::MapView* vOld, VIEW_NAMESPACE::MapView* vNew);
    void updateGridValues(VIEW_NAMESPACE::MapView2D* view);
    void updateRightGridValues(VIEW_NAMESPACE::MapView2D* view);
    void updateCurrentGridValues(VIEW_NAMESPACE::MapView2D* view);
    void updateCurrentRightGridValues(VIEW_NAMESPACE::MapView2D* view);

    bool    m_bActive;
    QPoint  m_PointBegin;
    QPoint  m_PointCurrent;
    QPoint  m_PointLast;
    QPoint  m_RightPointBegin;
    QPoint  m_RightPointCurrent;
    QPoint  m_RightPointLast;
    QVector2D m_vecGridBegin;
    QVector2D m_vecGridCurrent;
    QVector2D m_vecGridLast;
    QVector2D m_vecRightGridBegin;
    QVector2D m_vecRightGridCurrent;
    QVector2D m_vecRightGridLast;
    bool    m_bInDrag;
    bool    m_bInRightDrag;
    QPointer<VIEW_NAMESPACE::MapView> m_pView;
    bool    m_bInMouseOver;
    QPointer<MODEL_NAMESPACE::MapDocument> m_pDocument;
    QString m_szToolName;
    bool    m_bSnapping;
    bool    m_bGridCoords;
    bool    m_bRightGridCoords;
};

TOOLS_END_NAMESPACE

#endif // BASETOOL_H
