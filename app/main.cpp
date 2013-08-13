#include <QApplication>

#include "mainwin.h"
#include "logwindow.h"
#include "commandlineparser.h"
#include "globals.h"
#include <QString>
#include <QGLFormat>

#include "face.h" // TEMP
#include "viewport2d.h" // TEMP

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

    // Create global command line parser.
    g_pCmdLine = new CommandLineParser();
    g_pCmdLine->ParseArguments(argc, argv);

    g_pWindowTracker = new QList<MainWin*>();

    // Create main window and log window
    // (not shown if not debugging).
    g_pLog = new LogWindow();
    LogMessage(QString("Crowbar Editor - Last build %0 at %1").arg(__DATE__).arg(__TIME__));
    LogTaggedMessage("LOG", "Log Window Initialised.");

    MainWin win;
    win.setCentralWidget(NULL);

    win.show();
    g_pLog->show(); // show() checks for debugging mode automatically.

    int ret = app.exec();  // exec() starts the event loop. No user interaction should occur before this, but computation is fine.

    if ( g_pCmdLine ) delete g_pCmdLine;
    if ( g_pLog ) delete g_pLog;

    return ret;
}
