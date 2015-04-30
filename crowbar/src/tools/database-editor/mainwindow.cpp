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

void MainWindow::insertRowBefore()
{
    QModelIndex current = ui->treeView->currentIndex();
    if ( !current.isValid() )
    {
        m_pDatabase->insertRow(0, QModelIndex());
        return;
    }

    m_pDatabase->insertRow(current.row(), current.parent());
}

void MainWindow::insertRowAfter()
{
    QModelIndex current = ui->treeView->currentIndex();
    if ( !current.isValid() )
    {
        m_pDatabase->insertRow(m_pDatabase->rowCount(QModelIndex()), QModelIndex());
        return;
    }

    m_pDatabase->insertRow(current.row()+1, current.parent());
}

void MainWindow::removeRow()
{
    QModelIndex current = ui->treeView->currentIndex();
    if ( !current.isValid() ) return;

    m_pDatabase->removeRow(current.row(), current.parent());
}

void MainWindow::newSubkey()
{
    QModelIndex current = ui->treeView->currentIndex();
    m_pDatabase->insertRow(0, current);
}
