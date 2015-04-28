#include "orthographicsubwindow.h"
#include "ui_orthographicsubwindow.h"
#include "mapview2d.h"
#include <QKeyEvent>
#include <QFocusEvent>

using namespace VIEW_NAMESPACE;

OrthographicSubWindow::OrthographicSubWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrthographicSubWindow)
{
    ui->setupUi(this);
    ui->rbFront->installEventFilter(this);
    ui->rbTop->installEventFilter(this);
    ui->rbSide->installEventFilter(this);
}

OrthographicSubWindow::~OrthographicSubWindow()
{
    delete ui;
}

void OrthographicSubWindow::setTop()
{
    ui->centralView->setDirection(MapView2D::Top);
}

void OrthographicSubWindow::setFront()
{
    ui->centralView->setDirection(MapView2D::Front);
}

void OrthographicSubWindow::setSide()
{
    ui->centralView->setDirection(MapView2D::Side);
}

MapView2D* OrthographicSubWindow::view()
{
    return ui->centralView;
}

const MapView2D* OrthographicSubWindow::view() const
{
    return ui->centralView;
}
