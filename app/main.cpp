#include <QApplication>
#include <QString>
#include <QGLFormat>
#include <QObject>

#include "globals.h"
#include "mainwin.h"
#include "logwindow.h"
#include "commandlineparser.h"
#include "wr_listedcommandmanager.h"
#include "wr_commandinterpreter.h"

void initSystems(int argc, char **argv);
void shutdownSystems();
void debugTests();

DEFINE_CONVAR( test_var_1, "0", NULL, "Test var", 0, false, 0.0, false, 0.0)
DEFINE_CONVAR( test_var_2, "0", NULL, "Test var", 0, false, 0.0, false, 0.0)
DEFINE_CONVAR( test_var_3, "0", NULL, "Test var", 0, false, 0.0, false, 0.0)
DEFINE_CONVAR( test_var_4, "0", NULL, "Test var", 0, false, 0.0, false, 0.0)
DEFINE_CONVAR( test_var_5, "0", NULL, "Test var", 0, false, 0.0, false, 0.0)

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
    // Create global console command manager.
    g_pCommandManager = new ListedCommandManager(g_pCommandList);
    
    // Create global interpreter and hook up to the manager.
    g_pCommandInterpreter = new CommandInterpreter(g_pCommandManager);
    
    // Create global command line parser.
    g_pCmdLine = new CommandLineParser();
    g_pCmdLine->ParseArguments(argc, argv);

    // Create window tracker.
    g_pWindowTracker = new QList<MainWin*>();
    
    // Create console window.
    g_pLog = new LogWindow();
    
    // Connect manager's output to console window.
    g_pLog->connect(g_pCommandInterpreter, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)), g_pLog, SLOT(printMessage(CommandSenderInfo::OutputType,QString)));
    
    // Connect command box's command string to interpreter's parse function.
    g_pLog->connect(g_pLog, SIGNAL(commandString(QString)), g_pCommandInterpreter, SLOT(parse(QString)));
    
    // Connect command box's suggestion request to interpreter's suggestion retrieval function.
    g_pLog->connect(g_pLog, SIGNAL(getSuggestions(QString,QList<CommandInterpreter::CommandIdentPair>&,int)), g_pCommandInterpreter, SLOT(getSuggestions(QString,QList<CommandInterpreter::CommandIdentPair>&,int)));
    
    LogMessage(QString("Crowbar Editor - Last build %0 at %1").arg(__DATE__).arg(__TIME__));
}

void shutdownSystems()
{
    if ( g_pLog )
    {
        g_pLog->close();
        delete g_pLog;
    }
    if ( g_pWindowTracker ) delete g_pWindowTracker;
    if ( g_pCmdLine ) delete g_pCmdLine;
    if ( g_pCommandInterpreter ) delete g_pCommandInterpreter;
    if ( g_pCommandManager ) delete g_pCommandManager;
}

void debugTests()
{
}
