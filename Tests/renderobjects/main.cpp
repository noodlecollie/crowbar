#include "mainwindow.h"
#include <QApplication>
#include "glviewport.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    GLViewport* viewport = new GLViewport();
    w.setCentralWidget(viewport);
    w.show();
    
    return a.exec();
}
