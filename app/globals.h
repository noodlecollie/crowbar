/*! \file globals.h
 * \brief Defines global variables and classes.
 *
 * This file defines certain classes and variables which should be visible from anywhere in the application.
 * Examples include the command-line parser, the log window and the window tracker.
 * Some classes or properties have convenience macros defined.
 */

/**
 * \defgroup GlobalVariables Global variables
 * @{
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QList>
#include "commandsenderinfo.h"

// See globals.cpp for implementations.

//! \def DEFINE_CONVAR
//! \brief Macro for defining a ConVar with the default command manager and list.
//! \param szName_NoQuote Desired name of the ConVar, without quotes.
//! \param szDefVal Default value of the ConVar.
//! \param pCallback Optional callback (pass NULL if not needed).
//! \param szDesc Description of ConVar.
//! \param iFlags ConVar flags.
//! \param bMin Whether the ConVar has a min bound.
//! \param flMin Min value of the ConVar. If \a bMin is false, this value can be arbitrary.
//! \param bMax Whether the ConVar has a max bound.
//! \param flMax Max value of the ConVar. If \a bMax is false, this value can be arbitrary.

#define DEFINE_CONVAR( szName_NoQuote, szDefVal, pCallback, szDesc, pArgCallback, iFlags, bMin, flMin, bMax, flMax) \
ConVar szName_NoQuote(#szName_NoQuote, szDefVal, g_pCommandManager, &g_pCommandList, pCallback, szDesc, pArgCallback, iFlags, bMin, flMin, bMax, flMax);

//! \def DEFINE_CONCOMMAND
//! \brief Macro for defining a ConCommand with the default command manager and list.
//! \param szName_NoQuote Desired name of the ConVar, without quotes.
//! \param pCallback Pointer to callback.
//! \param szDesc Description of ConVar.
//! \param iFlags ConVar flags.

#define DEFINE_CONCOMMAND( szName_NoQuote, pCallback, szDesc, pArgCallback, iFlags ) \
ConCommand szName_NoQuote(#szName_NoQuote, pCallback, g_pCommandManager, &g_pCommandList, szDesc, pArgCallback, iFlags);

class ConsoleWindow;
class QString;
class CommandLineParser;
class MainWin;
class ListedCommandManager;
class ListedConsoleCommand;
class CommandInterpreter;
class ConCommand;
class ConVar;
class GlobalOutputRedirector;

//extern ConVar g_debugging;
//extern ConVar g_logging;

// =============== General ===============
/**
 * @brief Global command-line parser object.
 *
 * The command-line parser is created before the application windows are initialised and holds global application properties.
 */
extern CommandLineParser* g_pCmdLine;

/**
 * @brief Creates a basic one-time modal message box with simple message and an "OK" button.
 * @param message Message to display.
 */
extern void ShowMessageBox(QString message);

/**
 * @brief Creates a basic one-time modal error box with simple message and an "OK" button.
 * @param message Message to display.
 */
extern void ShowErrorBox(QString message);

// ========== Logging/Debugging ==========
/**
 * @brief Global console window object.
 *
 * Shows debug output from application components.
 */
extern ConsoleWindow* g_pLog;

/**
 * @brief Logs a message to the log window.
 *
 * Messages can be formatted by passing QString("%0 %1 ...").arg(arg0).arg(arg1)... as the message parameter.
 * @param message Message to write.
 * @param newline Whether a newline should follow the message. Defaults to true.
 */
extern void LogMessage(const QString &message, bool newline = true);

/**
 * @brief Logs a tagged message
 *
 * Messages can be formatted by passing QString("%0 %1 ...").arg(arg0).arg(arg1)... as the message parameter.<br>
 * A tagged message includes a short tag enclosed in square brackets, in order to help more clearly describe
 * what the message relates to.
 * @param tag Message tag.
 * @param message Message to display.
 * @param newline Whether a newline should follow the message. Defaults to true.
 */
extern void LogTaggedMessage(const QString &tag, const QString &message, bool newline = true);

/**
 * @brief Logs a warning to the log window. Log text is printed red and in bold.
 *
 * Messages can be formatted by passing QString("%0 %1 ...").arg(arg0).arg(arg1)... as the message parameter.
 * @param message Message to write.
 * @param newline Whether a newline should follow the message. Defaults to true.
 */
extern void LogWarning(const QString &message, bool newline = true);

/**
 * @brief Logs a tagged warning to the log window. Log text is printed red and in bold.
 *
 * Messages can be formatted by passing QString("%0 %1 ...").arg(arg0).arg(arg1)... as the message parameter.<br>
 * A tagged message includes a short tag enclosed in square brackets, in order to help more clearly describe
 * what the message relates to.
 * @param tag Message tag.
 * @param message Message to write.
 * @param newline Whether a newline should follow the message. Defaults to true.
 */
extern void LogTaggedWarning(const QString &tag, const QString &message, bool newline = true);

/**
 * @brief Logs output of the specified type to the log window.
 * @param type Type of output.
 * @param message Message to write.
 * @param newline Whether a newline should follow the message. Defaults to true.
 */
extern void LogOutput(CommandSenderInfo::OutputType type, const QString &message, bool newline = true);

/**
 * @brief Logs tagged output of the specified type to the log window.
 *
 * Messages can be formatted by passing QString("%0 %1 ...").arg(arg0).arg(arg1)... as the message parameter.<br>
 * A tagged message includes a short tag enclosed in square brackets, in order to help more clearly describe
 * what the message relates to.
 * @param type Type of output.
 * @param tag Message tag.
 * @param message Message to write.
 * @param newline Whether a newline should follow the message. Defaults to true.
 */
extern void LogTaggedOutput(CommandSenderInfo::OutputType type, const QString &tag, const QString &message, bool newline = true);

// =========== Window tracking ===========
/**
 * @brief Global window tracker object.
 *
 * When the last application window closes, the log window should close as well. In order to facilitate this,
 * an application window registers itself to the window tracker on creation and deregisters on close.
 * If the closed window was the last in the list and the log window is still open, it is closed as well.
 */
extern QList<MainWin*>* g_pWindowTracker;

// =============== Console ===============

/**
 * @brief Global console command manager, created in main.cpp.
 *
 * Console command and variables should register to this manager.
 */
extern ListedCommandManager* g_pCommandManager;

/**
 * @brief Global console command interpreter, created in main.cpp.
 */
extern CommandInterpreter*  g_pCommandInterpreter;

/**
 * @brief Global console command list pointer.
 *
 * Any console commands or variables should register to this pointer, to be picked
 * up by the global manager on creation.
 */
extern ListedConsoleCommand* g_pCommandList;

//extern GlobalOutputRedirector* g_pOutputRedirect;

#endif // GLOBAL_H

/**@}*/
