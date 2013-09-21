#include "globals.h"
#include "logwindow.h"
#include "commandlineparser.h"
#include <QMessageBox>
#include "mainwin.h"
#include "wr_listedcommandmanager.h"

LogWindow*              g_pLog;
CommandLineParser*      g_pCmdLine;
QList<MainWin*>*        g_pWindowTracker;
ListedConsoleCommand*   g_pCommandList = NULL;
ListedCommandManager*   g_pCommandManager = NULL;

//DEFINE_CONVAR(g_debugging, "0", NULL, "Whether debugging is enabled or not.", 0, true, 0.0, true, 1.0)
//DEFINE_CONVAR(g_logging, "0", NULL, "Whether logging is enabled or not.", 0, true, 0.0, true, 1.0)

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
void LogMessage(QString message, bool newline)
{
    // Append a newline if we need to.
    if ( newline ) message.append("\r\n");
    g_pLog->printMessage(message);
}

// Logs a tagged message in the format: "[tag] message"
void LogTaggedMessage(QString tag, QString message, bool newline)
{
    QString str("[" + tag + "] " + message);
    LogMessage(str, newline);
}

// Logs a warning. Text is printed red and in bold.
void LogWarning(QString message, bool newline)
{
    // Append a newline if we need to.
    if ( newline ) message.append("\r\n");
    g_pLog->printWarning(message);
}

// Logs a tagged warning in the format: "[tag] message"
void LogTaggedWarning(QString tag, QString message, bool newline)
{
    QString str("[" + tag + "] " + message);
    LogWarning(str, newline);
}
