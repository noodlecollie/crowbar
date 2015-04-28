#ifndef TOOLEDITFACEDOCKWIDGET_H
#define TOOLEDITFACEDOCKWIDGET_H

#include <QDockWidget>
#include "tooleditfacedialog.h"
#include "tooleditface.h"
#include <QPointer>

namespace Ui {
class ToolEditFaceDockWidget;
}

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT ToolEditFaceDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit ToolEditFaceDockWidget(ToolEditFace* tool, QWidget *parent = 0);
    ~ToolEditFaceDockWidget();
    
private slots:
    void toolActivated();
    void showDialog();
    void updateDialog(MODEL_NAMESPACE::MapFace* face);
    void updateFaceAttributes();
    void applyCurrentTextureToAllSelectedFaces();
    
private:
    Ui::ToolEditFaceDockWidget *ui;
    QPointer<ToolEditFace> m_pTool;
    ToolEditFaceDialog* m_pDialogue;
    QPointer<MODEL_NAMESPACE::MapFace> m_pCurrentFace;
};

TOOLS_END_NAMESPACE

#endif // TOOLEDITFACEDOCKWIDGET_H
