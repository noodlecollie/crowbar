#ifndef TOOLSELECTDOCKWIDGET_H
#define TOOLSELECTDOCKWIDGET_H

#include <QDockWidget>
#include "toolselect.h"

namespace Ui {
class ToolSelectDockWidget;
}

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolSelectDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ToolSelectDockWidget(ToolSelect* tool, QWidget *parent = 0);
    ~ToolSelectDockWidget();

public slots:
    void updateUI();
    void updateTool();
    
private slots:
    void toolActivated();

private:
    Ui::ToolSelectDockWidget *ui;
    QPointer<ToolSelect> m_pTool;
    bool m_bBlockUpdates;
};

TOOLS_END_NAMESPACE

#endif // TOOLSELECTDOCKWIDGET_H
