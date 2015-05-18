#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arbitrarydataa.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Model A:
    m_pTreeA = new ModelTreeA(this);

    ArbitraryDataA* data = new ArbitraryDataA();
    data->setIntegerProperty(5);
    data->setStringProperty("High Contrast");
    data->setObjectName("Data");

    ArbitraryDataA* data2 = new ArbitraryDataA(data);
    data2->setIntegerProperty(10);
    data2->setStringProperty("Cyantific");
    data2->setObjectName("Data 2");

    ArbitraryDataA* data3 = new ArbitraryDataA(data2);
    data3->setIntegerProperty(20);
    data3->setStringProperty("Simian Mobile Disco");
    data3->setObjectName("Data 3");

    m_pTreeA->setRoot(data);
    ui->treeView->setModel(m_pTreeA);
}

MainWindow::~MainWindow()
{
    delete ui;
}
