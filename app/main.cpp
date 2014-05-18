#include <QApplication>
#include <QString>
#include <QGLFormat>
#include <QObject>
#include <QTextStream>
#include <QDir>
#include <QSplashScreen>
#include <QPixmap>

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

#define OPENGL_MIN_VERSION 2.0f

using namespace ICONSOLE_NAMESPACE;

void initSystems(int argc, char **argv, QSplashScreen *splash);
void shutdownSystems();
void qDebugIntercept(QtMsgType type, const QMessageLogContext &, const QString &msg);
bool checkOpenGLVersion(QString* error);
QSplashScreen* splashScreen(const QString &logofile);

#ifdef QT_DEBUG
void debugTests();
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    // Record the max OpenGL version the system supports in the gl_max_version ConVar.
    QString error;
    if ( !checkOpenGLVersion(&error) )
    {
        QString message = QString("Unable to launch application: %0").arg(error);
        qFatal(message.toLatin1().constData());
        ShowErrorBox(message);
        return 1;
    }
    
    // Check the max version is high enough.
    if ( gl_max_version.floatValue() < OPENGL_MIN_VERSION )
    {
        QString message = QString("Unable to launch application: Minimum OpenGL version of %0 not met by the system.").arg(OPENGL_MIN_VERSION);
        qFatal(message.toLatin1().constData());
        ShowErrorBox(message);
        return 1;
    }
    
    // Display the splash screen.
    QSplashScreen* splash = splashScreen(QApplication::applicationDirPath() + QString("/resource/crowbar_splash.png"));
    splash->show();
    
    // Create and initialise all core systems.
    initSystems(argc, argv, splash);
    
    // Perform any tests we need.
#ifdef QT_DEBUG
    debugTests();
#endif

    MainWin win;
    win.setCentralWidget(NULL);

    splash->close();
    if ( splash )
    {
        delete splash;
        splash = NULL;
    }
    
    win.show();
    int ret = app.exec();  // exec() starts the event loop. No user interaction should occur before this, but computation is fine.

    shutdownSystems();
    return ret;
}

void initSystems(int argc, char **argv, QSplashScreen* splash)
{
    if ( splash ) splash->showMessage("Initialising systems...", Qt::AlignCenter);
    QApplication::processEvents();
    
    // Set style sheet.
    applyStyleSheet();
    
    // Create global interpreter and hook up to the CommandStore manager.
    Q_ASSERT(COMMANDSTORE_NAMESPACE::g_pCommandManager);
    g_pCommandInterpreter = new CommandInterpreter(COMMANDSTORE_NAMESPACE::g_pCommandManager);
    
    // Set the base directory for the command manager.
    COMMANDSTORE_NAMESPACE::g_pCommandManager->setBaseDirectory(QApplication::applicationDirPath());
    
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

#ifdef QT_DEBUG
void debugTests()
{
}
#endif

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
    
// This saves me from having to write the damn thing out over and over.
#define CHECK_VERSION(_var, _glVersion, _valToSet) \
if ( (_var & QGLFormat::OpenGL_Version_##_glVersion) == QGLFormat::OpenGL_Version_##_glVersion ) \
{ \
gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY); \
gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), _valToSet); \
gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY); \
return true; \
}
    
    // The highest one reached here will be the max version.
    CHECK_VERSION(flags, 4_0, 4.0f)
    CHECK_VERSION(flags, 3_3, 3.3f)
    CHECK_VERSION(flags, 3_2, 3.2f)
    CHECK_VERSION(flags, 3_1, 3.1f)
    CHECK_VERSION(flags, 3_0, 3.0f)
    CHECK_VERSION(flags, 2_1, 2.1f)
    CHECK_VERSION(flags, 2_0, 2.0f)
    CHECK_VERSION(flags, 1_5, 1.5f)
    CHECK_VERSION(flags, 1_4, 1.4f)
    CHECK_VERSION(flags, 1_3, 1.3f)
    CHECK_VERSION(flags, 1_2, 1.2f)
    CHECK_VERSION(flags, 1_1, 1.1f)

#undef CHECK_VERSION
    
    // We matched none.
    if ( error ) *error = "System does not support OpenGL.";
    
    gl_max_version.removeFlag(NGlobalCmd::CMDFLAG_READONLY);
    gl_max_version.setValue(CommandSenderInfo(gl_max_version.name(), NULL, NULL), 0.0f);
    gl_max_version.setFlag(NGlobalCmd::CMDFLAG_READONLY);
    
    return false;
}

QSplashScreen* splashScreen(const QString &logofile)
{
    // Create a pixmap from the file path.
    QPixmap pixmap(logofile);
    
    // Create the splash screen with this pixmap.
    return new QSplashScreen(pixmap, Qt::WindowStaysOnTopHint);
}
