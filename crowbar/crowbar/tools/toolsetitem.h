#ifndef TOOLSETITEM_H
#define TOOLSETITEM_H

#include <QObject>
#include "tools_global.h"
#include <QPointer>

class QDockWidget;

TOOLS_BEGIN_NAMESPACE

class BaseTool;

class TOOLSSHARED_EXPORT ToolSetItem : public QObject
{
    Q_OBJECT
public:
    explicit ToolSetItem(QObject *parent = 0);
    ~ToolSetItem();
    
    // Not owned by this object.
    BaseTool* tool() const;
    void setTool(BaseTool* tool);
    
    // Not owned by this object.
    QDockWidget* toolDockWidget() const;
    void setToolDockWidget(QDockWidget* widget);
    
signals:
    
public slots:
    
private:
    QPointer<BaseTool>      m_pTool;
    QPointer<QDockWidget>   m_pDockWidget;
};

TOOLS_END_NAMESPACE

#endif // TOOLSETITEM_H
