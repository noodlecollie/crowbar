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

//! \def DEBUGGING
//! \brief Convenience macro for gloabl CommandLineParser::Debugging() - returns whether debug mode is enabled.

//! \def LOGGING
//! \brief Convenience macro for gloabl CommandLineParser::Logging() - returns whether logging is enabled.

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
 * @brief Global log window object.
 *
 * Shows debug output from application components. The window still exists if debugging mode is not enabled but does not display,
 * and the Debug menu will not be present in any main application windows.
 */
extern LogWindow* g_pLog;

/**
 * @brief Logs a message to the log window.
 *
 * Messages can be formatted by passing QString("%0 %1 ...").arg(arg0).arg(arg1)... as the message parameter.
 * @param message Message to write.
 * @param newline Whether a newline should follow the message. Defaults to true.
 */
extern void LogMessage(QString message, bool newline = true);

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
extern void LogTaggedMessage(QString tag, QString message, bool newline = true);

/**
 * @brief Logs a warning to the log window. Log text is printed red and in bold.
 *
 * Messages can be formatted by passing QString("%0 %1 ...").arg(arg0).arg(arg1)... as the message parameter.
 * @param message Message to write.
 * @param newline Whether a newline should follow the message. Defaults to true.
 */
extern void LogWarning(QString message, bool newline = true);

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
extern void LogTaggedWarning(QString tag, QString message, bool newline = true);

// =========== Window tracking ===========
/**
 * @brief Global window tracker object.
 *
 * When the last application window closes, the log window should close as well. In order to facilitate this,
 * an application window registers itself to the window tracker on creation and deregisters on close.
 * If the closed window was the last in the list and the log window is still open, it is closed as well.
 */
extern QList<MainWin*>* g_pWindowTracker;

/**@}*/

#endif // GLOBAL_H
