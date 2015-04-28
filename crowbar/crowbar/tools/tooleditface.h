#ifndef TOOLEDITFACE_H
#define TOOLEDITFACE_H

#include "basetool.h"
#include <QPointer>
#include "mapface.h"

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolEditFace : public BaseTool
{
    Q_OBJECT
public:
    ToolEditFace(QObject* parent = 0);
    virtual ~ToolEditFace();
    
    virtual QString toolName() const;
    
signals:
    void faceSelected(MODEL_NAMESPACE::MapFace* face);
    
public slots:
    void applyCurrentTextureToAllSelectedFaces();
    
protected:
    virtual void processDragBegin(const QPoint &pos);
    virtual void processDragMove(const QPoint &pos);
    virtual void processDragEnd();
    virtual void processActivate();
    virtual void processDeactivate();
    virtual void processRightDragBegin(const QPoint &pos);
    
private:
    QPointer<MODEL_NAMESPACE::MapFace> m_pSelectedFace;
};

TOOLS_END_NAMESPACE

#endif // TOOLEDITFACE_H
