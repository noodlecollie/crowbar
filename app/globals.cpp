#include "globals.h"
#include "consolewindow.h"
#include "commandlineparser.h"
#include <QMessageBox>
#include "mainwin.h"
#include "wr_listedcommandmanager.h"
#include "commandsenderinfo.h"

using namespace ICONSOLE_NAMESPACE;

ConsoleWindow*          g_pLog = NULL;
CommandLineParser*      g_pCmdLine = NULL;
QList<MainWin*>*        g_pWindowTracker = NULL;
CommandInterpreter*     g_pCommandInterpreter = NULL;

// Creates a simple QMessageBox with an OK button that shows the specified message.
void ShowMessageBox(QString message)
{
    QMessageBox::information(NULL, "Crowbar", message);
}

void ShowErrorBox(QString message)
{
    QMessageBox::critical(NULL, "Crowbar", message);
}

// Logs a message, optionally excluding the terminating newline.
void LogMessage(const QString &message, bool newline)
{
    if ( !g_pLog ) return;
    
    // Append a newline if we need to.
    const QString &m = newline ? QString(message + "\r\n") : message;
    g_pLog->printToConsole(CommandSenderInfo::OutputGeneral, m);
}

// Logs a tagged message in the format: "[tag] message"
void LogTaggedMessage(const QString &tag, const QString &message, bool newline)
{
    LogMessage("[" + tag + "] " + message, newline);
}

// Logs a warning. Text is printed red and in bold.
void LogWarning(const QString &message, bool newline)
{
    if ( !g_pLog ) return;
    
    // Append a newline if we need to.
    const QString &m = newline ? QString(message + "\r\n") : message;
    g_pLog->printToConsole(CommandSenderInfo::OutputWarning, m);
}

// Logs a tagged warning in the format: "[tag] message"
void LogTaggedWarning(const QString &tag, const QString &message, bool newline)
{
    LogWarning("[" + tag + "] " + message, newline);
}

void LogOutput(CommandSenderInfo::OutputType type, const QString &message, bool newline)
{
    if ( !g_pLog ) return;
    
    // Append a newline if we need to.
    const QString &m = newline ? QString(message + "\r\n") : message;
    g_pLog->printToConsole(type, m);
}

// Logs a tagged warning in the format: "[tag] message"
void LogTaggedOutput(CommandSenderInfo::OutputType type, const QString &tag, const QString &message, bool newline)
{
    LogOutput(type, "[" + tag + "] " + message, newline);
}
