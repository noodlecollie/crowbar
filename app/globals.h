#ifndef GLOBAL_H
#define GLOBAL_H

#include <QList>

// See globals.cpp for implementations.

// Convnience macros for checking debug states - should be called after app.exec()!
#define DEBUGGING       g_pCmdLine->Debugging()
#define LOGGING         g_pCmdLine->Logging()

class LogWindow;
class QString;
class CommandLineParser;
class MainWin;

// =============== General ===============
extern CommandLineParser* g_pCmdLine;

extern void ShowMessageBox(QString);    // Creates basic one-time modal message box.

// ========== Logging/Debugging ==========
extern LogWindow* g_pLog;

// Logs a message.
extern void LogMessage(QString message, bool newline = true);

// Logs a tagged message. Tag is "[tag]".
extern void LogTaggedMessage(QString tag, QString message, bool newline = true);

// Logs a warning. Text is printed red and in bold.
extern void LogWarning(QString message, bool newline = true);

// Logs a tagged warning. Tag is "[tag]".
extern void LogTaggedWarning(QString tag, QString message, bool newline = true);

// =========== Window tracking ===========
extern QList<MainWin*>* g_pWindowTracker;

#endif // GLOBAL_H
