#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace MODEL_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pDatabase = new Database(this);
    ui->treeView->setModel(m_pDatabase);
}

MainWindow::~MainWindow()
{
    delete ui;
}
