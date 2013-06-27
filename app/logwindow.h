#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>

#define LOG_ZOOM_INCREMENT  2
#define LOG_WARNING_WEIGHT  75

class QTextEdit;
class QBoxLayout;
class QCloseEvent;
class QToolBar;
class QAction;
class QFile;
class QTextStream;
class LogWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();

    qreal getFontSize();
    
signals:
    
public slots:
    void show();
    void showAndRaise();
    void printMessage(QString);
    void logMessage(QString);
    void printWarning(QString);
    void logWarning(QString);
    void zoomIn();
    void zoomOut();
    void newLogFile();
    void newLogFile(QString);

private:
    QTextEdit*      m_pText;            // Main text window
    QBoxLayout*     m_pLayout;          // Widhet layout
    QToolBar*       m_pToolBar;         // Toolbar
    QAction*        m_pActLargeText;    // Zoom in
    QAction*        m_pActSmallText;    // Zoom out
    QFile*          m_pLogFile;         // Current log file
    QTextStream*    m_pLogStream;       // Log file text stream

    bool OpenLogFile(QString);
    void CloseLogFile();
    void KillLogStream();
    void KillLogFile();
};

#endif // LOGWINDOW_H
