/** \file consolewindow.h
 * \brief Defines the ConsoleWindow class for showing a developer console.
 */

#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QWidget>
#include "commandsenderinfo.h"
#include "iconsole_global.h"

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;

ICONSOLE_BEGIN_NAMESPACE
class ConsoleWidget;
class CommandEntryBox;
class CommandInterpreter;
ICONSOLE_END_NAMESPACE

/**
 * @brief Ties together the main elements from the IConsole library to create a functioning console window.
 */
class ConsoleWindow : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     * @param interpreter Command interpreter to link to.
     * @param parent QWidget parent, if applicable.
     */
    explicit ConsoleWindow(ICONSOLE_NAMESPACE::CommandInterpreter* interpreter, QWidget *parent = 0);
    
signals:
    
public slots:
    /**
     * @brief Prints a message to the console.
     * @param type Type of message.
     * @param message Message to print.
     */
    void printToConsole(ICONSOLE_NAMESPACE::CommandSenderInfo::OutputType type, const QString &message);
    
    /**
     * @brief Shows the logging window and raises it to be on top of any other windows.
     */
    void showAndRaise();
    
private:
    /**
     * @brief Connects up with the given CommandInterpreter.
     * @param interpreter Interpreter to link to.
     */
    void init(ICONSOLE_NAMESPACE::CommandInterpreter* interpreter);
    
    QVBoxLayout*                            m_pMainLayout;      /**< Vertical window layout. */
    QHBoxLayout*                            m_pSubLayout;       /**< Bottom layout for text box and submit button. */
    ICONSOLE_NAMESPACE::ConsoleWidget*      m_pConsoleWidget;   /**< Console widget. */
    ICONSOLE_NAMESPACE::CommandEntryBox*    m_pEntryBox;        /**< Entry box. */
    QPushButton*                            m_pSubmitButton;    /**< "Submit" button. */
};

#endif // CONSOLEWINDOW_H
