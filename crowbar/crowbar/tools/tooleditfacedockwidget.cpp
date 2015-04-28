#include "tooleditfacedockwidget.h"
#include "ui_tooleditfacedockwidget.h"
#include "mapdocument.h"

TOOLS_BEGIN_NAMESPACE

ToolEditFaceDockWidget::ToolEditFaceDockWidget(ToolEditFace *tool, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ToolEditFaceDockWidget)
{
    Q_ASSERT(tool);
    m_pTool = tool;
    ui->setupUi(this);
    setWindowTitle(tool->toolName());
    m_pDialogue = new ToolEditFaceDialog(this);
    m_pDialogue->setWindowFlags(m_pDialogue->windowFlags() | Qt:: WindowStaysOnTopHint);
    hide();
    
    connect(m_pTool.data(), &ToolEditFace::activated, this, &ToolEditFaceDockWidget::toolActivated);
    connect(m_pTool.data(), &ToolEditFace::deactivated, this, &ToolEditFaceDockWidget::hide);
    connect(ui->btnOpenDialogue, &QPushButton::clicked, this, &ToolEditFaceDockWidget::showDialog);
    connect(m_pTool.data(), &ToolEditFace::faceSelected, this, &ToolEditFaceDockWidget::updateDialog);
    connect(m_pDialogue, &ToolEditFaceDialog::attributesChanged, this, &ToolEditFaceDockWidget::updateFaceAttributes);
}

ToolEditFaceDockWidget::~ToolEditFaceDockWidget()
{
    delete ui;
}

void ToolEditFaceDockWidget::toolActivated()
{
    m_pCurrentFace = NULL;
    m_pDialogue->getAttributesFrom(NULL);
    show();
}

void ToolEditFaceDockWidget::showDialog()
{
    m_pDialogue->show();
}

void ToolEditFaceDockWidget::updateDialog(Model::MapFace *face)
{
    m_pCurrentFace = face;
    m_pDialogue->getAttributesFrom(m_pCurrentFace.isNull() ? NULL : m_pCurrentFace->textureInfo());
}

void ToolEditFaceDockWidget::updateFaceAttributes()
{
    if ( m_pCurrentFace.isNull() ) return;
    m_pDialogue->setAttributesOn(m_pCurrentFace->textureInfo());
}

void ToolEditFaceDockWidget::applyCurrentTextureToAllSelectedFaces()
{
    m_pTool->applyCurrentTextureToAllSelectedFaces();
}

TOOLS_END_NAMESPACE
