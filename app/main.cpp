#include <QApplication>
#include <QString>
#include <QGLFormat>
#include <QObject>
#include <QTextStream>
#include <QDir>
#include <QOpenGLContext>
#include <QOffscreenSurface>

#include "globals.h"
#include "mainwin.h"
#include "commandlineparser.h"
#include "wr_listedcommandmanager.h"
#include "wr_commandinterpreter.h"
#include "consolewindow.h"
#include "nglobalcmd.h"
#include "localcommands.h"
#include "commandsenderinfo.h"

#define LOG_QDEBUG_TAG      " Q "
#define LOG_QWARNING_TAG    " Q!"
#define LOG_QCRITICAL_TAG   "!Q!"
#define LOG_QFATAL_TAG      "XXX"

using namespace NIConsole;

void initSystems(int argc, char **argv);
void shutdownSystems();
void debugTests();
void qDebugIntercept(QtMsgType type, const QMessageLogContext &, const QString &msg);
bool checkOpenGLVersion(QString* error);

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QString error;
    if ( !checkOpenGLVersion(&error) )
    {
        QString message = QString("Unable to launch application: %0").arg(error);
        qFatal(message.toLatin1().constData());
        ShowErrorBox(message);
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
    // Set style sheet.
    applyStyleSheet();
    
    // Create global console command manager.
    //g_pCommandManager = new ListedCommandManager(g_pCommandList); // This is now in the CommandStore module.
    
    // Create global interpreter and hook up to the manager.
    g_pCommandInterpreter = new CommandInterpreter(NCommandStore::g_pCommandManager);
    
    // Create global command line parser.
    g_pCmdLine = new CommandLineParser();
    g_pCmdLine->ParseArguments(argc, argv);

    // Create window tracker.
    g_pWindowTracker = new QList<MainWin*>();
    
    // Create console window.
    g_pLog = new ConsoleWindow(g_pCommandInterpreter);
    g_pLog->resize(640, 480);
    
    // Set up message handler to print qDebug messages to console as well.
    qInstallMessageHandler(qDebugIntercept);
    
    // This won't necessarily reflect the right date/time if main.cpp is not modified before compile!
    //LogMessage(QString("Crowbar Editor - Last build %0 at %1").arg(__DATE__).arg(__TIME__));
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
    
    // Do we still need to delete this now that it's handled in the CommandStore module?
    //if ( g_pCommandManager ) delete g_pCommandManager;
}

void debugTests()
{
}

void qDebugIntercept(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    switch (type)
    {
        case QtDebugMsg:
        {
            LogTaggedMessage(LOG_QDEBUG_TAG, msg);
            QTextStream(stdout) << msg << endl;
            break;
        }
        case QtWarningMsg:
        {
            LogTaggedWarning(LOG_QWARNING_TAG, msg);
            QTextStream(stderr) << msg << endl;
            break;
        }
        case QtCriticalMsg:
        {
            LogTaggedWarning(LOG_QCRITICAL_TAG, msg);
            QTextStream(stderr) << msg << endl;
            break;
        }
        case QtFatalMsg:
        {
            LogTaggedWarning(LOG_QFATAL_TAG, msg);
            QTextStream(stderr) << msg << endl;
            abort();
        }
    }
}

// Returns true if the program should continue to run.
// Also updates gl_max_version.
bool checkOpenGLVersion(QString* error)
{
    if ( !QGLFormat::hasOpenGL() )
    {
        if ( error ) *error = "System does not support OpenGL.";
        
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 0.0f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        
        return false;
    }
    
    QGLFormat::OpenGLVersionFlags flags = QGLFormat::openGLVersionFlags();
    
#define GLFORMAT(_ver) QGLFormat::OpenGL_Version_##_ver
    
    // The highest one reached here will be the max version.
    if ( (flags & GLFORMAT(4_0)) == GLFORMAT(4_0) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 4.0f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(3_3)) == GLFORMAT(3_3) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 3.3f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(3_2)) == GLFORMAT(3_2) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 3.2f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(3_1)) == GLFORMAT(3_1) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 3.1f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(3_0)) == GLFORMAT(3_0) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 3.0f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(2_1)) == GLFORMAT(2_1) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 2.1f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(2_0)) == GLFORMAT(2_0) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 2.0f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(1_5)) == GLFORMAT(1_5) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 1.5f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(1_4)) == GLFORMAT(1_4) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 1.4f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(1_3)) == GLFORMAT(1_3) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 1.3f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(1_2)) == GLFORMAT(1_2) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 1.2f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
    if ( (flags & GLFORMAT(1_1)) == GLFORMAT(1_1) )
    {
        gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
        gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 1.1f);
        gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
        return true;
    }
    
#undef GLFORMAT
    
    // We matched none.
    if ( error ) *error = "System does not support OpenGL.";
    
    gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
    gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 0.0f);
    gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
    
    return false;
}
