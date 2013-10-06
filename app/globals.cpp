#include "globals.h"
#include "logwindow.h"
#include "commandlineparser.h"
#include <QMessageBox>
#include "mainwin.h"
#include "wr_listedcommandmanager.h"
#include "commandsenderinfo.h"

LogWindow*              g_pLog;
CommandLineParser*      g_pCmdLine;
QList<MainWin*>*        g_pWindowTracker;
ListedConsoleCommand*   g_pCommandList = NULL;
ListedCommandManager*   g_pCommandManager = NULL;
CommandInterpreter*     g_pCommandInterpreter = NULL;
//GlobalOutputRedirector* g_pOutputRedirect = NULL;

// Creates a simple QMessageBox with an OK button that shows the specified message.
void ShowMessageBox(QString message)
{
//    QMessageBox box;
//    box.setMinimumSize(300, 200);
//    box.setText(message);
//    box.exec();
    // Oops! Qt already had this as a static function.
    QMessageBox::information(NULL, "Crowbar", message);
}

void ShowErrorBox(QString message)
{
    QMessageBox::critical(NULL, "Crowbar", message);
}

// Logs a message, optionally excluding the terminating newline.
void LogMessage(const QString &message, bool newline)
{
    // Append a newline if we need to.
    //if ( newline ) message.append("\r\n");
    const QString &m = newline ? QString(message + "\r\n") : message;
    g_pLog->printMessage(CommandSenderInfo::OutputGeneral, m);
}

// Logs a tagged message in the format: "[tag] message"
void LogTaggedMessage(const QString &tag, const QString &message, bool newline)
{
    //const QString str("[" + tag + "] " + message);
    //LogMessage(str, newline);
    
    LogMessage("[" + tag + "]" + message, newline);
}

// Logs a warning. Text is printed red and in bold.
void LogWarning(const QString &message, bool newline)
{
    // Append a newline if we need to.
    //if ( newline ) message.append("\r\n");
    const QString &m = newline ? QString(message + "\r\n") : message;
    g_pLog->printMessage(CommandSenderInfo::OutputWarning, m);
}

// Logs a tagged warning in the format: "[tag] message"
void LogTaggedWarning(const QString &tag, const QString &message, bool newline)
{
    //const QString str("[" + tag + "] " + message);
    //LogWarning(str, newline);
    LogMessage("[" + tag + "]" + message, newline);
}
