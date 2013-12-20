#include <QApplication>
#include <QString>
#include <QGLFormat>
#include <QObject>
#include <QTextStream>
#include <QDir>

// TEMP

// TEMP

#include "globals.h"
#include "mainwin.h"
#include "commandlineparser.h"
#include "wr_listedcommandmanager.h"
#include "wr_commandinterpreter.h"
#include "consolewindow.h"
#include "nglobalcmd.h"

#define LOG_QDEBUG_TAG      " Q "
#define LOG_QWARNING_TAG    " Q!"
#define LOG_QCRITICAL_TAG   "!Q!"
#define LOG_QFATAL_TAG      "XXX"

void initSystems(int argc, char **argv);
void shutdownSystems();
void debugTests();
void qDebugIntercept(QtMsgType type, const QMessageLogContext &, const QString &msg);

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
    // Apply style sheet.
    QDir dir(qApp->applicationDirPath());
    if ( dir.cd("resource") && dir.exists("manifest.qss") )
    {
        QFile stylesheet(dir.filePath("manifest.qss"));
        stylesheet.open(QFile::ReadOnly);
        QString setSheet = QLatin1String(stylesheet.readAll());
        qApp->setStyleSheet(setSheet);
    }
    
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
    g_pLog = new ConsoleWindow(g_pCommandInterpreter);
    
    // Set up message handler to print qDebug messages to console as well.
    //qInstallMessageHandler(qDebugIntercept);
   
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
