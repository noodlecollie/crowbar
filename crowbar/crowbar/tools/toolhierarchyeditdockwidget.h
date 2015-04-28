#ifndef TOOLHIERARCHYEDITDOCKWIDGET_H
#define TOOLHIERARCHYEDITDOCKWIDGET_H

#include <QDockWidget>
#include "hierarchyeditdialogue.h"
#include "tooledithierarchy.h"

namespace Ui {
class ToolHierarchyEditDockWidget;
}

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolHierarchyEditDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit ToolHierarchyEditDockWidget(ToolEditHierarchy* tool, QWidget *parent = 0);
    ~ToolHierarchyEditDockWidget();
    
private slots:
    void showDialog();
    void toolActivated();
    void updateDialog();
    void handleDocumentChanged(MODEL_NAMESPACE::MapDocument* document);
    
private:
    Ui::ToolHierarchyEditDockWidget *ui;
    HierarchyEditDialogue* m_pDialogue;
    QPointer<ToolEditHierarchy> m_pTool;
};

TOOLS_END_NAMESPACE

#endif // TOOLHIERARCHYEDITDOCKWIDGET_H
