#include "toolselectdockwidget.h"
#include "ui_toolselectdockwidget.h"
#include <QtDebug>

TOOLS_BEGIN_NAMESPACE

ToolSelectDockWidget::ToolSelectDockWidget(ToolSelect* tool, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ToolSelectDockWidget)
{
    m_bBlockUpdates = false;
    
    Q_ASSERT(tool);
    m_pTool = tool;
    ui->setupUi(this);
    setWindowTitle(tool->toolName());
    hide();
    
    connect(m_pTool.data(), &ToolSelect::selectionOperationChanged, this, &ToolSelectDockWidget::updateUI);
    connect(m_pTool.data(), &ToolSelect::activated, this, &ToolSelectDockWidget::toolActivated);
    connect(m_pTool.data(), &ToolSelect::deactivated, this, &ToolSelectDockWidget::hide);
}

ToolSelectDockWidget::~ToolSelectDockWidget()
{
    delete ui;
}

void ToolSelectDockWidget::toolActivated()
{
    show();
    updateTool();
}

void ToolSelectDockWidget::updateUI()
{
    if ( m_bBlockUpdates || m_pTool.isNull() ) return;

    switch ( m_pTool->selectionOperation() )
    {
    case ToolSelect::Union:
        ui->cbbSelectionType->setCurrentIndex(1);
        break;

    case ToolSelect::Subtract:
        ui->cbbSelectionType->setCurrentIndex(2);
        break;

    default:
        ui->cbbSelectionType->setCurrentIndex(0);
        break;
    }
}

void ToolSelectDockWidget::updateTool()
{
    if ( m_pTool.isNull() ) return;
    
    m_bBlockUpdates = true;

    switch ( ui->cbbSelectionType->currentIndex() )
    {
    case 1:
        m_pTool->setSelectionOperation(ToolSelect::Union);
        break;

    case 2:
        m_pTool->setSelectionOperation(ToolSelect::Subtract);
        break;

    default:
        m_pTool->setSelectionOperation(ToolSelect::Replace);
        break;
    }
    
    m_bBlockUpdates = false;
}

TOOLS_END_NAMESPACE
