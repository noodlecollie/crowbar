#include <QApplication>
#include <QString>
#include <QFile>
#include <QDir>
#include <QPushButton>
#include <QVBoxLayout>
#include "consolewidget.h"

QWidget* mainWin = NULL;
QPushButton* button = NULL;
QVBoxLayout* vb = NULL;
NIConsole::ConsoleWidget* console = NULL;

void init();

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QDir dir(app.applicationDirPath());
    if ( dir.cd("resource") && dir.exists("manifest.qss") )
    {
        QFile stylesheet(dir.filePath("manifest.qss"));
        stylesheet.open(QFile::ReadOnly);
        QString setSheet = QLatin1String(stylesheet.readAll());
        app.setStyleSheet(setSheet);
    }
    
    init();
    mainWin->show();

    return app.exec();
}

void init()
{
    mainWin = new QWidget();
    vb = new QVBoxLayout(mainWin);
    mainWin->setLayout(vb);
    button = new QPushButton();
    button->setText("A test button");
    vb->addWidget(button);
    console = new NIConsole::ConsoleWidget();
    vb->addWidget(console);
}
