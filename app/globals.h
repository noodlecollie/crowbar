#ifndef GLOBAL_H
#define GLOBAL_H

// See globals.cpp for implementations.

// Convnience macro for checking debug state - should be called after app.exec()!
#define DEBUGGING       g_pCmdLine->Debugging()

class LogWindow;
class QString;
class CommandLineParser;
class MainWin;

// =============== General ===============
extern CommandLineParser* g_pCmdLine;

extern void ShowMessageBox(QString);    // Creates basic one-time modal message box.

// ========== Logging/Debugging ==========
extern LogWindow* g_pLog;

#endif // GLOBAL_H
