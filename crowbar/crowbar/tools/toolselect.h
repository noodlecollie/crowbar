#ifndef TOOLSELECT_H
#define TOOLSELECT_H

#include "basetool.h"
#include <QVector2D>
#include <QVector3D>
#include "brushvertexoperator.h"
#include <QSet>

VIEW_BEGIN_NAMESPACE
class MapDocumentView;
VIEW_END_NAMESPACE

MODEL_BEGIN_NAMESPACE
class MapComponent;
MODEL_END_NAMESPACE

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolSelect : public BaseTool
{
    Q_OBJECT
    Q_PROPERTY(SelectionOperation selectionOperation READ selectionOperation WRITE setSelectionOperation NOTIFY selectionOperationChanged)
    Q_PROPERTY(bool dragCrosshairEnabled READ dragCrosshairEnabled WRITE setDragCrosshairEnabled NOTIFY dragCrosshairStateChanged)
    Q_PROPERTY(bool dragBoxEnabled READ dragBoxEnabled WRITE setDragBoxEnabled NOTIFY dragBoxStateChanged)
    Q_PROPERTY(bool mouseOverCrosshairEnabled READ mouseOverCrosshairEnabled WRITE setMouseOverCrosshairEnabled NOTIFY mouseOverCrosshairStateChanged)
public:
    explicit ToolSelect(QObject *parent = 0);
    virtual ~ToolSelect();

    enum SelectionOperation
    {
        Replace,    // Replace document's selected set with selected components.
        Union,      // Add selected components to document's set.
        Subtract,   // Subtract selected components from document's set.
    };

    SelectionOperation selectionOperation() const;
    void setSelectionOperation(SelectionOperation operation);

    bool dragCrosshairEnabled() const;
    void setDragCrosshairEnabled(bool enabled);

    bool dragBoxEnabled() const;
    void setDragBoxEnabled(bool enabled);

    bool mouseOverCrosshairEnabled() const;
    void setMouseOverCrosshairEnabled(bool enabled);

    virtual QString toolName() const;

    bool controlModifier() const;

protected:
    virtual void processDragBegin(const QPoint &pos);
    virtual void processDragEnd();
    virtual void processDragMove(const QPoint &pos);
    virtual void processRightDragBegin(const QPoint &pos);
    virtual void processMouseOverBegin(const QPoint &pos);
    virtual void processMouseOverMove(const QPoint &pos);
    virtual void processMouseOverEnd();
    virtual void processActivate();
    virtual void processDeactivate();

signals:
    void selectionOperationChanged(SelectionOperation);
    void documentViewChanged(VIEW_NAMESPACE::MapDocumentView*);
    void mouseOverCrosshairStateChanged(bool);
    void dragCrosshairStateChanged(bool);
    void dragBoxStateChanged(bool);
    void controlModifierChanged(bool);

public slots:
    void commandDelete(bool keyDown);
    void setControlModifier(bool enabled);

private slots:
    void handleActiveViewChanged(VIEW_NAMESPACE::MapView* view);

private:
    void performOperation(const QList<MODEL_NAMESPACE::MapComponent*> &list);
    void visualEffectsForDragBegin(const QPoint &pos);
    void visualEffectsForDragMove(const QPoint &pos);
    void visualEffectsForDragEnd();
    void visualEffectsForMouseOverBegin(const QPoint &pos);
    void visualEffectsForMouseOverMove(const QPoint &pos);
    void visualEffectsForMouseOverEnd();
    bool pointWithinSelectedSetBounds(const QPoint &pos);

    SelectionOperation  m_iOperation;
    QPointer<VIEW_NAMESPACE::MapDocumentView> m_pDocView;
    bool    m_bShouldMoveDrag;
    bool    m_bDragCrosshair;
    bool    m_bDragBox;
    bool    m_bMouseOverCrosshair;
    QPoint  m_CachedMouseOverPos;
    bool    m_bHaveDragged;
    bool    m_bControlModifier;
    
    QVector3D m_vecHandleTransX;
    QVector3D m_vecHandleTransY;
    QVector3D m_vecHandleAnchor;
    QVector3D m_vecHandleBasePos;
    BrushVertexOperator* m_pOperator;

    // Probably not the best way to do this...
    QSet<VIEW_NAMESPACE::MapView*> m_EncounteredViews;
};

TOOLS_END_NAMESPACE

#endif // TOOLSELECT_H
