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
 * Part of the application can send log messages which will be printed to this window and logged into a file.
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

    /**
     * @brief Returns the name of the current log file.
     * @return Name of the log file, or "" if no file is currently open.
     */
    QString GetLogFileName() const;
    
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
     * @brief Writes a message to a log file (but does not print to the log window).
     * @param message Message to print.
     */
    void logMessage(QString message);

    /**
     * @brief Prints a warning to the log window (but not to a log file).
     * @param message Message to print.
     */
    void printWarning(QString message);

    /**
     * @brief Writes a warning to a log file (but does not print to the log window).
     * @param message Message to print.
     */
    void logWarning(QString message);

    /**
     * @brief Makes the log window text larger.
     */
    void zoomIn();

    /**
     * @brief Makes the log window text smaller.
     */
    void zoomOut();

    /**
     * @brief Creates a new log file whose name is based on the current date and time.
     */
    void newLogFile();

    /**
     * @brief Creates a new log file.
     * @param filename Name of file to create.
     */
    void newLogFile(QString filename);

private:
    QTextEdit*      m_pText;            /**< Main text window */
    QBoxLayout*     m_pLayout;          /**< Widhet layout */
    QToolBar*       m_pToolBar;         /**< Toolbar */
    QAction*        m_pActLargeText;    /**< Zoom in */
    QAction*        m_pActSmallText;    /**< Zoom out */
    QFile*          m_pLogFile;         /**< Current log file */
    QTextStream*    m_pLogStream;       /**< Log file text stream */

    /**
     * @brief Opens a log file with the specified name and returns whether the operation was successful.
     * @param filename Name of file to open.
     * @return True if opening was successful, otherwise false.
     */
    bool OpenLogFile(QString filename);

    /**
     * @brief Closes the log file if it is open (wrapper for KillLogStream() + KillLogFile()).
     */
    void CloseLogFile();

    /**
     * @brief Kills the file stream for the current log file.
     */
    void KillLogStream();

    /**
     * @brief Kills the log file object for the current log file.
     */
    void KillLogFile();
};

#endif // LOGWINDOW_H
