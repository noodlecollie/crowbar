#include "mainwindow.h"
#include "maporiginmarker.h"
#include "mapdocument.h"
#include <QtDebug>
#include "mapbrushbuilder.h"
#include "mapdocumentrenderer.h"
#include <QDockWidget>

using namespace VIEW_NAMESPACE;
using namespace MODEL_NAMESPACE;
using namespace TOOLS_NAMESPACE;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_pView(new OpenGLWidgetV2(this)), m_pDockAddWidget(new QDockWidget("Add brush", this)),
      m_pAddWidget(new BrushAddWidget())
{
    setCentralWidget(m_pView);


    m_pDoc = new MapDocument(this);
    m_pView->documentRenderer()->setDocument(m_pDoc);
    m_pView->setHighlight(true);
    m_pView->setHighlightWidth(5.0f);

    m_pDockAddWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_pDockAddWidget->setFeatures(QDockWidget::DockWidgetMovable);
    m_pDockAddWidget->setWidget(m_pAddWidget);
    addDockWidget(Qt::LeftDockWidgetArea, m_pDockAddWidget);

    connect(m_pAddWidget, SIGNAL(createBrush(QVector3D,QVector3D)), this, SLOT(createBrush(QVector3D,QVector3D)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::createBrush(const QVector3D &min, const QVector3D &max)
{
    MapBrushBuilder bldr;
    MapBrush* b = bldr.axisAlignedBrush(min, max);
    m_pDoc->brushes().append(b);
}
