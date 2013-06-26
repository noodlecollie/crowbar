#include <QApplication>

#include "mainwin.h"
#include "logwindow.h"
#include "commandlineparser.h"
#include "globals.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // Create global command line parser.
    g_pCmdLine = new CommandLineParser();
    g_pCmdLine->ParseArguments(argc, argv);

    // Create main window and log window
    // (not shown if not debugging).
    g_pLog = new LogWindow();
    MainWin win;

    win.show();
    g_pLog->show(); // show() checks for debugging mode automatically.

    int ret = app.exec();  // exec() starts the event loop. No user interaction should occur before this, but computation is fine.

    if ( g_pCmdLine ) delete g_pCmdLine;
    if ( g_pLog ) delete g_pLog;

    return ret;
}
