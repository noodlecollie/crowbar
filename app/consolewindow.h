/** \file consolewindow.h
 * \brief Defines the ConsoleWindow class for showing a developer console.
 */

#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QWidget>
#include "commandsenderinfo.h"

class QVBoxLayout;
class QHBoxLayout;
class ConsoleWidget;
class CommandEntryBox;
class QPushButton;
class CommandInterpreter;

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
    explicit ConsoleWindow(CommandInterpreter* interpreter, QWidget *parent = 0);
    
signals:
    
public slots:
    /**
     * @brief Prints a message to the console.
     * @param type Type of message.
     * @param message Message to print.
     */
    void printToConsole(CommandSenderInfo::OutputType type, const QString &message);
    
    /**
     * @brief Shows the logging window and raises it to be on top of any other windows.
     */
    void showAndRaise();
    
private:
    /**
     * @brief Connects up with the given CommandInterpreter.
     * @param interpreter Interpreter to link to.
     */
    void init(CommandInterpreter* interpreter);
    
    QVBoxLayout*        m_pMainLayout;      /**< Vertical window layout. */
    QHBoxLayout*        m_pSubLayout;       /**< Bottom layout for text box and submit button. */
    ConsoleWidget*      m_pConsoleWidget;   /**< Console widget. */
    CommandEntryBox*    m_pEntryBox;        /**< Entry box. */
    QPushButton*        m_pSubmitButton;    /**< "Submit" button. */
};

#endif // CONSOLEWINDOW_H
