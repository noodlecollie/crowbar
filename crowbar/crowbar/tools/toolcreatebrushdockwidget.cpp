#include "toolcreatebrushdockwidget.h"
#include "ui_toolcreatebrushdockwidget.h"

TOOLS_BEGIN_NAMESPACE

ToolCreateBrushDockWidget::ToolCreateBrushDockWidget(ToolCreateBrush *tool, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ToolCreateBrushDockWidget)
{
    Q_ASSERT(tool);
    m_pTool = tool;
    ui->setupUi(this);
    setWindowTitle(tool->toolName());
    hide();
    
    connect(m_pTool.data(), &ToolCreateBrush::activated, this, &ToolCreateBrushDockWidget::toolActivated);
    connect(m_pTool.data(), &ToolCreateBrush::deactivated, this, &ToolCreateBrushDockWidget::hide);
}

ToolCreateBrushDockWidget::~ToolCreateBrushDockWidget()
{
    delete ui;
}

void ToolCreateBrushDockWidget::toolActivated()
{
    show();
}

TOOLS_END_NAMESPACE
