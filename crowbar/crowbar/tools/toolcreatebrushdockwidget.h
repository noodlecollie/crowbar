#ifndef TOOLCREATEBRUSHDOCKWIDGET_H
#define TOOLCREATEBRUSHDOCKWIDGET_H

#include <QDockWidget>
#include "toolcreatebrush.h"

namespace Ui {
class ToolCreateBrushDockWidget;
}

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolCreateBrushDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit ToolCreateBrushDockWidget(ToolCreateBrush* tool, QWidget *parent = 0);
    ~ToolCreateBrushDockWidget();
    
private slots:
    void toolActivated();
    
private:
    Ui::ToolCreateBrushDockWidget *ui;
    QPointer<ToolCreateBrush> m_pTool;
};

TOOLS_END_NAMESPACE

#endif // TOOLCREATEBRUSHDOCKWIDGET_H
