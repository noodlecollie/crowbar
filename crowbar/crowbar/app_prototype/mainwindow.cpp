#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applicationdata.h"
#include "mapbrushbuilder.h"
#include "model_util.h"
#include <QtDebug>
#include <QGLSceneNode>
#include "brushlistdialogue.h"
#include "transformdialogue.h"
#include "basetool.h"
#include "maplightpositional.h"
#include <QDir>
#include <QImage>
#include <QGLTexture2D>
#include <QFileDialog>
#include "toolbasicsquare.h"

using namespace MODEL_NAMESPACE;
using namespace VIEW_NAMESPACE;
using namespace TOOLS_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(appData(), SIGNAL(documentListChanged()), this, SLOT(updateDocumentMenu()));

    updateDocumentMenu();

    m_p2DView = new MapView2D(this);
    m_p3DView = new MapView3D(this);
    
    m_p2DView->setDirection(MapView2D::Top);
    m_p2DView->setObjectName("Top view");
    
    m_pSplitter = new QSplitter(this);
    
    m_pSplitter->addWidget(m_p3DView);
    m_pSplitter->addWidget(m_p2DView);
    
    setCentralWidget(m_pSplitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDocumentMenu()
{
    ui->menuDocuments->clear();

    ApplicationData* apd = appData();

    if ( apd->documentList().count() < 1 )
    {
        QAction* a = ui->menuDocuments->addAction(QString("No documents loaded"));
        a->setEnabled(false);
        return;
    }

    for ( int i = 0; i < apd->documentList().count(); i++ )
    {
        QAction* a = ui->menuDocuments->addAction(QString("Doc: 0x%0").arg((qlonglong)apd->documentList().at(i)));
        connect(a, SIGNAL(triggered()), this, SLOT(handleDocumentMenuItemChosen()));
    }
}

void MainWindow::createNewDocument()
{
    MapDocument* doc = appData()->createDocument();

    for ( int i = 0; i < 1; i++ )
    {
        QVector3D vec( Model_Util::getRandomFloat(-256, 256),
                       Model_Util::getRandomFloat(-256, 256),
                       Model_Util::getRandomFloat(-64, 64) );
        
        QVector3D min = QVector3D(-32,-32,-32);
        QVector3D max = QVector3D(32,32,32);
        QVector3D min2(-5,-5,-5);
        QVector3D max2(5,5,5);

        MapBrush* brush = MapBrushBuilder::axisAlignedBrush(min, max, NULL);
        brush->setObjectName(QString("Brush %0").arg(i));

        brush->hierarchyTransform()->setTranslation(vec);

        brush->setParentComponent(doc->world());
        brush->setColor(QColor(255,128,64));
        
        MapBrush* childBrush = MapBrushBuilder::axisAlignedBrush(min2, max2, NULL);
        childBrush->setObjectName(QString("ChildBrush %0").arg(i));
        childBrush->setParentComponent(brush);
        childBrush->hierarchyTransform()->setTranslation(QVector3D(40,0,0));
        childBrush->setColor(QColor(128,0,0));
        
        MapBrush* proxyBrush = MapBrushBuilder::axisAlignedBrush(min2, max2, NULL);
        proxyBrush->setObjectName(QString("ProxyBrush %0").arg(i));
        proxyBrush->setProxyTarget(brush);
        proxyBrush->setParentComponent(brush->parentComponent());

        proxyBrush->hierarchyTransform()->setTranslation(QVector3D(10,10,32));

        // Testing these:
        ToolBasicSquare* square = new ToolBasicSquare(brush);
        square->setSize(10.0f);
        square->setBorderWidth(3.0f);
        square->setBorderColor(QColor(55,137,72));
        square->setFixedBillboard(true);
        square->setObjectName(QString("ToolBasicSquare %0").arg(i));
        square->hierarchyTransform()->setTranslation(QVector3D(-60, -60, 0));
    }
    
    MapLightPositional* light = new MapLightPositional();
    light->setParentComponent(doc->world());
    light->hierarchyTransform()->setTranslation(QVector3D(0, 0, 128));

    ui->menuDocuments->actions().at(appData()->documentList().indexOf(doc))->trigger();
}

void MainWindow::handleDocumentMenuItemChosen()
{
    QAction* action = qobject_cast<QAction*>(QObject::sender());
    if ( !action ) return;

    // We determine the document index by the index of the action within the menu.
    // This is probably a terrible idea but it's early days and I need a quick solution.
    int index = ui->menuDocuments->actions().indexOf(action);
    if ( index < 0 ) return;

    MapDocument* doc = appData()->documentList().at(index);
    Q_ASSERT(doc);

    if ( !m_pCurrentDocument.isNull() )
    {
        disconnect(m_pCurrentDocument.data(), SIGNAL(renderUpdateHint()), this, SLOT(updateViews()));
    }

    m_pCurrentDocument = doc;

    if ( !m_pCurrentDocument.isNull() )
    {
        connect(m_pCurrentDocument.data(), SIGNAL(renderUpdateHint()), this, SLOT(updateViews()));
    }

    MapDocumentCollection* col = appData()->documentData(doc);
    if ( !col )
    {
        if ( !m_pCurrentTool.isNull() )
        {
            disconnect(m_p2DView, SIGNAL(dragBegin(QPoint)), m_pCurrentTool.data(), SLOT(dragBegin(QPoint)));
            disconnect(m_p2DView, SIGNAL(dragMove(QPoint)), m_pCurrentTool.data(), SLOT(dragMove(QPoint)));
            disconnect(m_p2DView, SIGNAL(dragEnd()), m_pCurrentTool.data(), SLOT(dragEnd()));

            disconnect(m_p2DView, SIGNAL(mouseOverBegin(QPoint)), m_pCurrentTool.data(), SLOT(mouseOverBegin(QPoint)));
            disconnect(m_p2DView, SIGNAL(mouseOverMove(QPoint)), m_pCurrentTool.data(), SLOT(mouseOverMove(QPoint)));
            disconnect(m_p2DView, SIGNAL(mouseOverEnd()), m_pCurrentTool.data(), SLOT(mouseOverEnd()));

            m_pCurrentTool->deactivate();
            m_pCurrentTool->setActiveView(NULL);
        }

        m_pCurrentTool = NULL;
    }
    else
    {
        if ( !m_pCurrentTool.isNull() )
        {
            disconnect(m_p2DView, SIGNAL(dragBegin(QPoint)), m_pCurrentTool.data(), SLOT(dragBegin(QPoint)));
            disconnect(m_p2DView, SIGNAL(dragMove(QPoint)), m_pCurrentTool.data(), SLOT(dragMove(QPoint)));
            disconnect(m_p2DView, SIGNAL(dragEnd()), m_pCurrentTool.data(), SLOT(dragEnd()));

            disconnect(m_p2DView, SIGNAL(mouseOverBegin(QPoint)), m_pCurrentTool.data(), SLOT(mouseOverBegin(QPoint)));
            disconnect(m_p2DView, SIGNAL(mouseOverMove(QPoint)), m_pCurrentTool.data(), SLOT(mouseOverMove(QPoint)));
            disconnect(m_p2DView, SIGNAL(mouseOverEnd()), m_pCurrentTool.data(), SLOT(mouseOverEnd()));

            m_pCurrentTool->deactivate();
            m_pCurrentTool->setActiveView(NULL);
        }

        m_pCurrentTool = col->toolList().at(0);

        if ( !m_pCurrentTool.isNull() )
        {
            connect(m_p2DView, SIGNAL(dragBegin(QPoint)), m_pCurrentTool.data(), SLOT(dragBegin(QPoint)));
            connect(m_p2DView, SIGNAL(dragMove(QPoint)), m_pCurrentTool.data(), SLOT(dragMove(QPoint)));
            connect(m_p2DView, SIGNAL(dragEnd()), m_pCurrentTool.data(), SLOT(dragEnd()));

            connect(m_p2DView, SIGNAL(mouseOverBegin(QPoint)), m_pCurrentTool.data(), SLOT(mouseOverBegin(QPoint)));
            connect(m_p2DView, SIGNAL(mouseOverMove(QPoint)), m_pCurrentTool.data(), SLOT(mouseOverMove(QPoint)));
            connect(m_p2DView, SIGNAL(mouseOverEnd()), m_pCurrentTool.data(), SLOT(mouseOverEnd()));

            m_pCurrentTool->setActiveView(m_p2DView);
            m_pCurrentTool->activate();
        }
    }

    currentDocumentUpdated();
}

void MainWindow::currentDocumentUpdated()
{
    if ( m_pCurrentDocument.isNull() ) return;

    MapDocumentCollection* collection = appData()->documentData(m_pCurrentDocument.data());
    IRender* r = collection->documentView();

    m_p2DView->setRenderable(r);
    m_p3DView->setRenderable(r);
    updateViews();
}

void MainWindow::showBrushDialogue()
{
    BrushListDialogue* dlg = new BrushListDialogue(m_pCurrentDocument.data(), this);
    dlg->show();
}

void MainWindow::showTransformDialogue()
{
    TransformDialogue* dlg = new TransformDialogue(m_pCurrentDocument.data(), this);
    dlg->show();
}

void MainWindow::updateViews()
{
    m_p2DView->update();
    m_p3DView->update();
}

void MainWindow::setTopView()
{
    m_p2DView->setDirection(MapView2D::Top);
}

void MainWindow::setFrontView()
{
    m_p2DView->setDirection(MapView2D::Front);
}

void MainWindow::setSideView()
{
    m_p2DView->setDirection(MapView2D::Side);
}

void MainWindow::loadTextures()
{
    m_szTextureDir = QFileDialog::getExistingDirectory(this, "Select a directory:", qApp->applicationDirPath());
}
