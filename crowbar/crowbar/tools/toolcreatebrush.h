#ifndef TOOLCREATEBRUSH_H
#define TOOLCREATEBRUSH_H

#include <QObject>
#include "basetool.h"
#include <QVector3D>
#include "mapview2d.h"
#include "model_util.h"
#include <QPointer>
#include "toolwireframebox.h"

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolCreateBrush : public BaseTool
{
    Q_OBJECT
public:
    explicit ToolCreateBrush(QObject *parent = 0);
    virtual ~ToolCreateBrush();

    virtual QString toolName() const;
    
signals:
    
public slots:
    
private slots:
    void handleActiveViewChanged(VIEW_NAMESPACE::MapView* view);
    
protected:
    virtual void processDragBegin(const QPoint &pos);
    virtual void processDragMove(const QPoint &pos);
    virtual void processDragEnd();
    virtual void processExecute();
    virtual void processDeactivate();
    
private:
    
    void visualEffectsForDragBegin(const QPoint &pos);
    void visualEffectsForDragMove(const QPoint &pos);
    void visualEffectsForDragEnd();
    
    void setBounds(const QVector3D &v1, const QVector3D &v2);
    static void depthInterval(VIEW_NAMESPACE::MapView2D::CameraDirection dir, const QVector3D &vMin, const QVector3D &vMax, float &min, float &max);
    static void setVectorComponent(VIEW_NAMESPACE::MapView2D::CameraDirection dir, QVector3D &vec, float val);
    
    // These are in grid space.
    QVector3D   m_vecMin;
    QVector3D   m_vecMax;

    QVector3D m_vecWorldMin;
    QVector3D m_vecWorldMax;

    QPointer<MODEL_NAMESPACE::ToolWireframeBox> m_pBox;
    QVector3D m_vecHandleBaseGridPos;
    QVector3D m_vecHandleAnchorGridPos;
    bool m_bPickedHandle;
    QBox3D m_CachedBox;
    bool m_bCentreHandle;
    bool m_bHandleTranslateOnX;
    
    // Probably not the best way to do this...
    QSet<VIEW_NAMESPACE::MapView*> m_EncounteredViews;
};

TOOLS_END_NAMESPACE

#endif // TOOLCREATEBRUSH_H
