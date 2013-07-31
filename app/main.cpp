#include <QApplication>

#include "mainwin.h"
#include "logwindow.h"
#include "commandlineparser.h"
#include "globals.h"
#include <QString>

#include "viewport2d.h" // TEMP

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

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

    // TEMP
    GLEnumList enables;
    enables << GL_DEPTH_TEST /*<< GL_CULL_FACE*/ << GL_MULTISAMPLE;
    GLenumPairList hints;
    OpenGLInitialiser init = {GL_SMOOTH, QColor(0, 255, 0, 255), enables, GL_LEQUAL, hints};
    Viewport2D* view = new Viewport2D();
    view->setInitialiser(init);
    win.setCentralWidget(view);
    // TEMP

    win.show();
    g_pLog->show(); // show() checks for debugging mode automatically.

    int ret = app.exec();  // exec() starts the event loop. No user interaction should occur before this, but computation is fine.

    if ( g_pCmdLine ) delete g_pCmdLine;
    if ( g_pLog ) delete g_pLog;

    return ret;
}
