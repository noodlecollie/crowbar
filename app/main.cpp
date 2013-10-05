#include <QApplication>
#include <QString>
#include <QGLFormat>

#include "globals.h"
#include "mainwin.h"
#include "logwindow.h"
#include "commandlineparser.h"
#include "wr_listedcommandmanager.h"

void initSystems(int argc, char **argv);
void shutdownSystems();
void debugTests();

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // Check for correct OpenGL version.
    // Qt supports the OpenGL core profile from version 3.1 upwards.
    if (!QGLFormat::openGLVersionFlags().testFlag(QGLFormat::OpenGL_Version_3_1))
    {
        ShowErrorBox("An OpenGL level of 3.1 or above is required but\nis not supported by the system.");
        return 1;
    }
    
    initSystems(argc, argv);
    
    debugTests();

    MainWin win;
    win.setCentralWidget(NULL);

    win.show();

    int ret = app.exec();  // exec() starts the event loop. No user interaction should occur before this, but computation is fine.

    shutdownSystems();
    return ret;
}

void initSystems(int argc, char **argv)
{
    // Create global console command
    g_pCommandManager = new ListedCommandManager(g_pCommandList);
    
    // Create global command line parser.
    g_pCmdLine = new CommandLineParser();
    g_pCmdLine->ParseArguments(argc, argv);

    // Create window tracker.
    g_pWindowTracker = new QList<MainWin*>();
    
    // Create main window and log window
    // (not shown if not debugging).
    g_pLog = new LogWindow();
    LogMessage(QString("Crowbar Editor - Last build %0 at %1").arg(__DATE__).arg(__TIME__));
}

void shutdownSystems()
{
    if ( g_pLog ) delete g_pLog;
    if ( g_pWindowTracker ) delete g_pWindowTracker;
    if ( g_pCmdLine ) delete g_pCmdLine;
    if ( g_pCommandManager ) delete g_pCommandManager;
}

void debugTests()
{
}
