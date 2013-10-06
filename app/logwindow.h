/*! \file logwindow.h
 * \brief Defines the logging window used when the application is in debug mode.
  */

#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>
#include "commandsenderinfo.h"
#include "commandinterpreter.h"

class ConsoleWidget;
class QVBoxLayout;
class QHBoxLayout;
class QCloseEvent;
class QToolBar;
class QAction;
class QFile;
class QTextStream;
class CommandEntryBox;
class QPushButton;
class CommandSuggestionList;

/**
 * @brief Logging window.
 *
 * TODO: Rewrite this documentation.
 * If the application is started in debug mode, the logging window will be shown as well as any application windows.
 * Parts of the application can send log messages which will be printed to this window and logged into a file.
 * The log window can be re-opened from the Debug menu in an application window if it is closed.
 * If logging is enabled without debug mode, any messages will be sent to a log file but the log window and the Debug
 * menu will not be shown.
 */
class LogWindow : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     * @param parent Parent object (usually NULL).
     */
    explicit LogWindow(QWidget *parent = 0);

    /**
      @brief Destructor.
      */
    virtual ~LogWindow();
    
signals:
    void commandString(const QString&);
    void getSuggestions(const QString&,QList<CommandInterpreter::CommandIdentPair>&,int);
    
public slots:

    /**
     * @brief Shows the logging window and raises it to be on top of any other windows.
     */
    void showAndRaise();
    
    void printMessage(CommandSenderInfo::OutputType type, const QString &message);

private:
    virtual void moveEvent(QMoveEvent *e);
    virtual void showEvent(QShowEvent *e);
    
    ConsoleWidget*          m_pText;            /**< Main text window */
    CommandEntryBox*        m_pEntry;
    QVBoxLayout*            m_pLayout;          /**< Widget layout */
    QHBoxLayout*            m_pBottomLayout;
    QPushButton*            m_pExecButton;
    CommandSuggestionList*  m_pSgList;
};

#endif // LOGWINDOW_H
