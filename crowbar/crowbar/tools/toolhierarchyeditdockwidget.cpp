#include "toolhierarchyeditdockwidget.h"
#include "ui_toolhierarchyeditdockwidget.h"
#include "mapdocument.h"

using namespace MODEL_NAMESPACE;

TOOLS_BEGIN_NAMESPACE

ToolHierarchyEditDockWidget::ToolHierarchyEditDockWidget(ToolEditHierarchy *tool, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ToolHierarchyEditDockWidget)
{
    Q_ASSERT(tool);
    m_pTool = tool;
    ui->setupUi(this);
    m_pDialogue = new HierarchyEditDialogue(this);
    m_pDialogue->setWindowFlags(m_pDialogue->windowFlags() | Qt:: WindowStaysOnTopHint);
    setWindowTitle(tool->toolName());
    hide();
    
    connect(m_pTool.data(), &ToolEditHierarchy::activated, this, &ToolHierarchyEditDockWidget::toolActivated);
    connect(m_pTool.data(), &ToolEditHierarchy::deactivated, this, &ToolHierarchyEditDockWidget::hide);
    connect(m_pTool.data(), &ToolEditHierarchy::documentChanged, this, &ToolHierarchyEditDockWidget::handleDocumentChanged);
    handleDocumentChanged(m_pTool->document());
}

ToolHierarchyEditDockWidget::~ToolHierarchyEditDockWidget()
{
    delete ui;
}

void ToolHierarchyEditDockWidget::showDialog()
{
    m_pDialogue->show();
}

void ToolHierarchyEditDockWidget::toolActivated()
{
    show();
}

void ToolHierarchyEditDockWidget::updateDialog()
{
    m_pDialogue->readValues();
}

void ToolHierarchyEditDockWidget::handleDocumentChanged(Model::MapDocument *document)
{
    MapDocument* pOld = m_pDialogue->document();
    if ( pOld )
    {
        disconnect(&pOld->selectedSet(), SIGNAL(itemAdded(MapComponent*)), this, SLOT(updateDialog()));
        disconnect(&pOld->selectedSet(), SIGNAL(itemChanged(MapComponent*)), this, SLOT(updateDialog()));
        disconnect(&pOld->selectedSet(), SIGNAL(itemRemoved(MapComponent*)), this, SLOT(updateDialog()));
    }
    
    m_pDialogue->setDocument(document);
    if ( document )
    {
        connect(&document->selectedSet(), SIGNAL(itemAdded(MapComponent*)), this, SLOT(updateDialog()));
        connect(&document->selectedSet(), SIGNAL(itemChanged(MapComponent*)), this, SLOT(updateDialog()));
        connect(&document->selectedSet(), SIGNAL(itemRemoved(MapComponent*)), this, SLOT(updateDialog()));
    }
    
    m_pDialogue->readValues();
}

TOOLS_END_NAMESPACE
