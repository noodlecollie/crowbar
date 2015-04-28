#include "mainwindow.h"
#include <QApplication>
#include "applicationsettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Initialise the application settings!
    appSettings();
    
    MainWindow w;
    w.show();

    return a.exec();
}
