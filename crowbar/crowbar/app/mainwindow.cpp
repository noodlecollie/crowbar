#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applicationsettings.h"
#include "toolsetitem.h"
#include "basetool.h"
#include <QtDebug>
#include <QAction>
#include <QtMath>
#include <QDockWidget>
#include "mapdocumentview.h"
#include "mapview2d.h"
#include "mapview3d.h"
#include <QFocusEvent>
#include <QCloseEvent>
#include "mapview3d.h"
#include <QFileDialog>
#include <QMessageBox>
#include "documentserialiser.h"
#include <QFileInfo>

using namespace MODEL_NAMESPACE;
using namespace TOOLS_NAMESPACE;
using namespace VIEW_NAMESPACE;
using namespace SERIALISATION_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pPlaceholderWidget = new QWidget(this);
    m_pPlaceholderLayout = new QVBoxLayout(m_pPlaceholderWidget);
    m_pPlaceholderWidget->setLayout(m_pPlaceholderLayout);
    setCentralWidget(m_pPlaceholderWidget);
    m_pSplitter = new QSplitter();
    m_pPlaceholderLayout->addWidget(m_pSplitter);
    m_p3DView = new MapView3D();
    m_pSubWindow = new OrthographicSubWindow();

    m_pSplitter->addWidget(m_p3DView);
    m_pSplitter->addWidget(m_pSubWindow);
    
    m_p3DView->setHighlightOnFocus(true);
    m_p3DView->setDrawWhenFocusLost(true);
    m_p3DView->setFocusOnMouseOver(true);
    
    m_pSubWindow->view()->setHighlightOnFocus(true);
    m_pSubWindow->view()->setDrawWhenFocusLost(true);
    m_pSubWindow->view()->setFocusOnMouseOver(true);

    m_pTreeWidget = new MapTreeWidgetDialog(this);
    m_pTreeWidget->treeWidget()->setWorldIcon(":/icons/world");
    m_pTreeWidget->treeWidget()->setBrushIcon(":/icons/block");
    m_pTreeWidget->treeWidget()->setComponentIcon(":/icons/component");
    
    connect(m_p3DView, &MapView3D::gainedFocus, this, &MainWindow::viewFocusGained);
    connect(m_p3DView, &MapView3D::lostFocus, this, &MainWindow::viewFocusLost);
    connect(m_pSubWindow->view(), &MapView2D::gainedFocus, this, &MainWindow::viewFocusGained);
    connect(m_pSubWindow->view(), &MapView2D::lostFocus, this, &MainWindow::viewFocusLost);
    
    m_pTexDockWidget = new TextureTableDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, m_pTexDockWidget);
    m_pTexDockWidget->updateTextureList(appSettings().loadedTextures().keys());
    connect(m_pTexDockWidget, SIGNAL(textureChanged(QString)), this, SLOT(changeCurrentTexture(QString)));
    
    m_iCurrentDocumentIndex = 0;
    m_iCurrentToolIndex = 0;
    setCurrentDocument(-1);
    updateStatusBar();

    m_pInputTranslator = new InputTranslator(this);
    setUpInputTranslator();
    
    updateDocumentMenuList();
    
    connect(&appSettings(), SIGNAL(documentCreated(Model::MapDocument*,View::MapDocumentView*)), this, SLOT(handleDocumentCreated(Model::MapDocument*)));
    connect(&appSettings(), SIGNAL(documentRemoved(Model::MapDocument*,View::MapDocumentView*)), this, SLOT(handleDocumentRemoved(Model::MapDocument*)));
    
    // Tools must be set up by this point!
    for ( int i = 0; i < appSettings().toolSetCount(); i++ )
    {
        QDockWidget* w = appSettings().toolSetItem(i)->toolDockWidget();
        w->hide();
        addDockWidget(Qt::LeftDockWidgetArea, w);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doCreateDocument()
{
    appSettings().createDocument();
}

void MainWindow::handleDocumentCreated(MapDocument *doc)
{
    updateDocumentMenuList();
    setCurrentDocument(appSettings().documentIndex(doc));
    updateStatusBar();
}

void MainWindow::handleDocumentRemoved(MapDocument*)
{
    updateDocumentMenuList();
    setCurrentDocument(-1); // TODO: Is there a better solution?
    updateStatusBar();
}

void MainWindow::updateDocumentMenuList()
{
    ui->menuDocuments->clear();
    
    if ( appSettings().documentCount() < 1 )
    {
        QAction* a = ui->menuDocuments->addAction("No documents loaded.");
        a->setEnabled(false);
        return;
    }
    else
    {
        int unnamedDocs = 0;
        for ( int i = 0; i < appSettings().documentCount(); i++ )
        {
            QString docTitle = appSettings().document(i)->displayName().trimmed();
            if ( docTitle.isNull() )
            {
                docTitle = QString("[Untitled %0]").arg(++unnamedDocs);
            }
            
            QAction* a = ui->menuDocuments->addAction(docTitle);
            
            // This is a horrible hack but it'll tell us the index of the selected document
            // when the action is chosen from the menu.
            a->setProperty("indexOfDocument", QVariant(i));
        }
    }
}

void MainWindow::chooseDocument(QAction *action)
{
    QVariant var = action->property("indexOfDocument");
    if ( !var.isValid() )
    {
        setCurrentDocument(-1);
        return;
    }
    
    bool ok = false;
    int index = var.toInt(&ok);
    if ( !ok )
    {
        setCurrentDocument(-1);
        return;
    }
    
    setCurrentDocument(index);
    updateStatusBar();
}

void MainWindow::closeCurrentDocument()
{
    if ( m_iCurrentDocumentIndex < 0 ) return;
    
    int prevIndex = m_iCurrentDocumentIndex;
    
    appSettings().removeDocument(m_iCurrentDocumentIndex);
    setCurrentDocument(-1);
    
    // Current document index is now -1.
    // Set new index appropriately depending on what we were before.
    if ( appSettings().documentCount() < 1 )
    {
        updateStatusBar();
        return;
    }
    if ( appSettings().documentCount() <= prevIndex ) setCurrentDocument(appSettings().documentCount() - 1);
    else setCurrentDocument(prevIndex);

    updateStatusBar();
}

void MainWindow::setCurrentDocument(int index)
{
    if ( index < -1 ) index = -1;
    else if ( index >= appSettings().documentCount() ) index = appSettings().documentCount() - 1;
    
    if ( m_iCurrentDocumentIndex == index ) return;
    
    int oldIndex = m_iCurrentDocumentIndex;
    m_iCurrentDocumentIndex = index;
    
    if ( m_iCurrentDocumentIndex < 0 )
    {
        handleCurrentDocumentChange(appSettings().document(oldIndex), NULL);
        return;
    }
    
    handleCurrentDocumentChange(appSettings().document(oldIndex), appSettings().document(m_iCurrentDocumentIndex));
}

void MainWindow::handleCurrentDocumentChange(MapDocument *vOld, MapDocument *vNew)
{
    if ( vOld )
    {
        // Nothing yet.
    }

    if ( vNew )
    {
        MapDocumentView* view = appSettings().documentView(appSettings().documentIndex(vNew));
        m_p3DView->setRenderable(view);
        m_pSubWindow->view()->setRenderable(view);
        vNew->setCurrentTexture(m_pTexDockWidget->currentTextureName());
        
        m_pSplitter->show();
        ui->mainToolBar->show();
        BaseTool* t = appSettings().toolSetItem(m_iCurrentToolIndex)->tool();
        t->setDocument(vNew);
        t->setActiveView(m_p3DView);
        t->activate();
    }
    else
    {
        m_p3DView->setRenderable(NULL);
        m_pSubWindow->view()->setRenderable(NULL);
        
        m_pSplitter->hide();
        ui->mainToolBar->hide();
        BaseTool* t = appSettings().toolSetItem(m_iCurrentToolIndex)->tool();
        t->deactivate();
        t->setDocument(NULL);
        t->setActiveView(NULL);
    }
}

void MainWindow::setUpInputTranslator()
{
    QAction* gridPowerUp = new QAction(this);
    connect(gridPowerUp, SIGNAL(triggered()), this, SLOT(increaseCurrentGridPower()));
    m_pInputTranslator->addKeyMapping(Qt::Key_BracketRight, gridPowerUp);

    QAction* gridPowerDown = new QAction(this);
    connect(gridPowerDown, SIGNAL(triggered()), this, SLOT(decreaseCurrentGridPower()));
    m_pInputTranslator->addKeyMapping(Qt::Key_BracketLeft, gridPowerDown);
    
    QAction* toggleSubWindow = new QAction(this);
    connect(toggleSubWindow, SIGNAL(triggered()), this, SLOT(toggleSecondaryView()));
    m_pInputTranslator->addKeyMapping(Qt::Key_Tab, toggleSubWindow);
    
    QAction* enableToolSnapping = new QAction(this);
    connect(enableToolSnapping, SIGNAL(triggered()), this, SLOT(enableCurrentToolSnapping()));
    m_pInputTranslator->addKeyMapping(Qt::Key_Alt, enableToolSnapping, false);
    
    QAction* disableToolSnapping = new QAction(this);
    connect(disableToolSnapping, SIGNAL(triggered()), this, SLOT(disableCurrentToolSnapping()));
    m_pInputTranslator->addKeyMapping(Qt::Key_Alt, disableToolSnapping);
    
    QAction* execTool = new QAction(this);
    connect(execTool, SIGNAL(triggered()), this, SLOT(executeCurrentTool()));
    m_pInputTranslator->addKeyMapping(Qt::Key_Return, execTool);

    m_pInputTranslator->addCommandMapping(Qt::Key_Delete, "commandDelete");
    m_pInputTranslator->addCommandMapping(Qt::Key_Control, "setControlModifier");
}

void MainWindow::increaseCurrentGridPower()
{
    MapDocument* doc = appSettings().document(m_iCurrentDocumentIndex);
    if ( !doc ) return;

    doc->increaseGridPower();
    m_pSubWindow->view()->setGridPower(doc->gridPower());
    m_pSubWindow->view()->update();
    m_pSubWindow->update();
    updateStatusBar();
}

void MainWindow::decreaseCurrentGridPower()
{
    MapDocument* doc = appSettings().document(m_iCurrentDocumentIndex);
    if ( !doc ) return;

    doc->decreaseGridPower();
    m_pSubWindow->view()->setGridPower(doc->gridPower());
    m_pSubWindow->view()->update();
    m_pSubWindow->update();
    updateStatusBar();
}

MapDocument* MainWindow::currentDocument() const
{
    return appSettings().document(m_iCurrentDocumentIndex);
}

void MainWindow::updateStatusBar()
{
    MapDocument* doc = currentDocument();
    QString message;
    if ( !doc )
    {
        message = QString("No document loaded.");
    }
    else
    {
        message = QString("Document: %0 | Grid size: %1")
                         .arg(doc->displayName().isNull() ? "[Untitled]" : doc->displayName())
                         .arg((int)qPow(2.0f, doc->gridPower()));
    }

    statusBar()->showMessage(message);
}

void MainWindow::toolChosen()
{
    if ( m_iCurrentDocumentIndex < 0 ) return;
    QAction* action = qobject_cast<QAction*>(QObject::sender());
    if ( !action ) return;

    // Crude, but...
    QString name = action->text();
    int index = appSettings().toolSetIndex(name);
    if ( index < 0 ) return;

    BaseTool* t = appSettings().toolSetItem(m_iCurrentToolIndex)->tool();
    t->deactivate();
    t->setDocument(NULL);
    t->setActiveView(NULL);

    m_iCurrentToolIndex = index;

    t = appSettings().toolSetItem(m_iCurrentToolIndex)->tool();
    t->setDocument(currentDocument());
    t->setActiveView(m_p3DView);
    m_pInputTranslator->setInputTarget(t);
    t->activate();
}

void MainWindow::toggleSecondaryView()
{
    m_pSubWindow->setVisible(!m_pSubWindow->isVisible());
    if ( m_p3DView->inMouseLook() ) m_p3DView->cancelMouseLook();
}

void MainWindow::viewFocusGained(MapView* view)
{
    view->installEventFilter(m_pInputTranslator);
    for ( int i = 0; i < appSettings().toolSetCount(); i++ )
    {
        BaseTool* t = appSettings().toolSetItem(i)->tool();
        t->setActiveView(view);
    }
}

void MainWindow::viewFocusLost(MapView* view)
{
    view->removeEventFilter(view);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    closeCurrentDocument();

    m_p3DView->makeCurrent();
    appSettings().notifyClosing();
    m_p3DView->doneCurrent();
    
    QMainWindow::closeEvent(e);
}

void MainWindow::enableCurrentToolSnapping()
{
    if ( m_iCurrentToolIndex < 0 ) return;
    
    BaseTool* t = appSettings().toolSetItem(m_iCurrentToolIndex)->tool();
    if ( t ) t->setSnapping(true);
}

void MainWindow::disableCurrentToolSnapping()
{
    if ( m_iCurrentToolIndex < 0 ) return;
    
    BaseTool* t = appSettings().toolSetItem(m_iCurrentToolIndex)->tool();
    if ( t ) t->setSnapping(false);
}

void MainWindow::executeCurrentTool()
{
    if ( m_iCurrentToolIndex < 0 ) return;
    
    BaseTool* t = appSettings().toolSetItem(m_iCurrentToolIndex)->tool();
    if ( t ) t->execute();
}

void MainWindow::saveCurrentDocumentAs()
{
    if ( m_iCurrentDocumentIndex < 0 ) return;

    QString currentPath = appSettings().filePath(m_iCurrentDocumentIndex);
    if ( currentPath.isEmpty() ) currentPath = qApp->applicationDirPath();

    QString file = QFileDialog::getSaveFileName(this, "Save file", currentPath, "VMF file (*.vmf)");
    if ( file.isEmpty() ) return;

    QFile saveFile(file);
    if ( !saveFile.open(QIODevice::WriteOnly) )
    {
        QMessageBox::critical(this, "Error", QString("File %0 could not be opened for writing.").arg(file));
        return;
    }

    QByteArray output;
    MapDocument* d = appSettings().document(m_iCurrentDocumentIndex);
    DocumentSerialiser::toVMF(d, output);
    saveFile.write(output);

    QFileInfo fileInfo(file);
    appSettings().setFilePath(m_iCurrentDocumentIndex, fileInfo.canonicalPath());
    d->setDisplayName(fileInfo.fileName());
}

void MainWindow::changeCurrentTexture(const QString &name)
{
    // This is sort of inefficient but I can't be bothered to hook it up intelligently right now.
    for ( int i = 0; i < appSettings().documentCount(); i++ )
    {
        appSettings().document(i)->setCurrentTexture(name);
    }
}

void MainWindow::showTreeView()
{
    MapDocument* doc = appSettings().document(m_iCurrentDocumentIndex);
    if ( !doc ) return;
    m_pTreeWidget->treeWidget()->setData(doc->world());
    m_pTreeWidget->show();
}
