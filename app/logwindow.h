/*! \file logwindow.h
 * \brief Defines the logging window used when the application is in debug mode.
  */

#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>

//! \def LOG_ZOOM_INCREMENT
//! \brief Increment with which to zoom in or out on LogWindow::zoomIn() or LogWindow::zoomOut().

//! \def LOG_WARNING_WEIGHT
//! \brief Text weight of warning messages printed to the log window.

#define LOG_ZOOM_INCREMENT  2
#define LOG_WARNING_WEIGHT  75

class QTextEdit;
class QBoxLayout;
class QCloseEvent;
class QToolBar;
class QAction;
class QFile;
class QTextStream;

/**
 * @brief Logging window.
 *
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
    ~LogWindow();
    
signals:
    
public slots:
    /**
     * @brief Shows the logging window.
     */
    void show();

    /**
     * @brief Shows the logging window and raises it to be on top of any other windows.
     */
    void showAndRaise();

    /**
     * @brief Prints a message to the log window (but not to a log file).
     * @param message Message to print.
     */
    void printMessage(QString message);

    /**
     * @brief Prints a warning to the log window (but not to a log file).
     * @param message Message to print.
     */
    void printWarning(QString message);

private:
    QTextEdit*      m_pText;            /**< Main text window */
    QBoxLayout*     m_pLayout;          /**< Widget layout */
    QToolBar*       m_pToolBar;         /**< Toolbar */
};

#endif // LOGWINDOW_H
