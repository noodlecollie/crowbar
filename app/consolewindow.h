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

class ConsoleWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleWindow(CommandInterpreter* interpreter, QWidget *parent = 0);
    
signals:
    
public slots:
    void printToConsole(CommandSenderInfo::OutputType type, const QString &message);
    
    /**
     * @brief Shows the logging window and raises it to be on top of any other windows.
     */
    void showAndRaise();
    
private:
    void init(CommandInterpreter* interpreter);
    
    QVBoxLayout*        m_pMainLayout;
    QHBoxLayout*        m_pSubLayout;
    ConsoleWidget*      m_pConsoleWidget;
    CommandEntryBox*    m_pEntryBox;
    QPushButton*        m_pSubmitButton;
};

#endif // CONSOLEWINDOW_H
