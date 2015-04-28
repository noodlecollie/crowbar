#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model_util.h"
#include "usercontrolledcamera.h"
#include <QSurfaceFormat>
#include "mapdocument.h"
#include "mapbrushbuilder.h"
#include "mapdocumentrenderer2.h"

using namespace VIEW_NAMESPACE;
using namespace MODEL_NAMESPACE;
using namespace TOOLS_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QSurfaceFormat format;
//    format.setDepthBufferSize(24);
//    format.setStencilBufferSize(8);
//    format.setVersion(3, 2);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    QSurfaceFormat::setDefaultFormat(format);

    m_pView = new MapView2D();
    m_pView->setCoordinateTransform(Model_Util::HammerToOpenGL());
    m_pView->setDirection(MapView2D::ZNeg);
    m_pView->setHighlightOnFocus(true);
    
    m_pDocument = new MapDocument(this);
    
//    MapBrush* brush = MapBrushBuilder::axisAlignedBrush(QVector3D(0,0,0), QVector3D(128,128,64));
//    m_pDocument->brushes().append(brush);
//    brush->setSelected(true);
    
    m_pRenderer = new MapDocumentRenderer2(m_pDocument);
    m_pRenderer->setSelectedColor(QColor(255,128,0));
    m_pView->setRenderable(m_pRenderer);
    
    ui->horizontalLayout->addWidget(m_pView);
    
    m_p3DView = new MapView3D();
    m_p3DView->setCoordinateTransform(Model_Util::HammerToOpenGL());
    m_p3DView->userCamera()->setTopSpeed(128);
    m_p3DView->setHighlightOnFocus(true);
    m_p3DView->setRenderable(m_pRenderer);
    ui->horizontalLayout->addWidget(m_p3DView);
}

void MainWindow::setDrawGrid(bool enabled)
{
    m_pView->setDrawGrid(enabled);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setXPos()
{
    m_pView->setDirection(MapView2D::XPos);
}

void MainWindow::setXNeg()
{
    m_pView->setDirection(MapView2D::XNeg);
}

void MainWindow::setYPos()
{
    m_pView->setDirection(MapView2D::YPos);
}

void MainWindow::setYNeg()
{
    m_pView->setDirection(MapView2D::YNeg);
}

void MainWindow::setZPos()
{
    m_pView->setDirection(MapView2D::ZPos);
}

void MainWindow::setZNeg()
{
    m_pView->setDirection(MapView2D::ZNeg);
}
