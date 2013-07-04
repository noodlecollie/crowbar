#include "logwindow.h"
#include <QTextEdit>
#include <QBoxLayout>
#include <QPixmap>
#include <QIcon>
#include <QToolBar>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include "globals.h"
#include "commandlineparser.h"

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent)
{
    // Set up window.
    setAttribute(Qt::WA_DeleteOnClose, false);  // We just want to hide when closed.
    //setWindowFlags(Qt::Tool);                 // This means the window does not show up on the taskbar - do we want this?

    setMinimumSize(320, 240);
    resize(640, 480);
    setWindowTitle("Log Output");

    // Set up layout within window.
    m_pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_pLayout->setMargin(2);

    //Set up toolbar and icons.
    m_pToolBar = new QToolBar();
    m_pLayout->addWidget(m_pToolBar);
    m_pActLargeText = m_pToolBar->addAction(QIcon(QPixmap("resource/log_zoomin.png")), "Zoom in", this, SLOT(zoomIn()));
    m_pActSmallText = m_pToolBar->addAction(QIcon(QPixmap("resource/log_zoomout.png")), "Zoom out", this, SLOT(zoomOut()));
//    m_pActLargeText = m_pToolBar->addAction("+", this, SLOT(zoomIn()));
//    m_pActSmallText = m_pToolBar->addAction("-", this, SLOT(zoomOut()));

    // Text widget
    m_pText = new QTextEdit();
    m_pLayout->addWidget(m_pText);
    m_pText->setReadOnly(true);                     // User cannot enter text but it can be added programmatically.
    m_pText->setWordWrapMode(QTextOption::WordWrap);
    m_pText->setLineWrapMode(QTextEdit::WidgetWidth);
    m_pText->setUndoRedoEnabled(false);             // We don't want undo/redo to occur.
    m_pText->setFontFamily("Lucida Console");

    // Log file
    m_pLogFile = NULL;
    m_pLogStream = NULL;

    if ( LOGGING )
    {
        newLogFile();
    }
}

LogWindow::~LogWindow()
{
    CloseLogFile();
}

QString LogWindow::GetLogFileName() const
{
    if ( !m_pLogFile ) return "";

    return m_pLogFile->fileName();
}

bool LogWindow::OpenLogFile(QString filename)
{
    CloseLogFile();

    m_pLogFile = new QFile(filename);

    if ( m_pLogFile->open(QIODevice::Append) )
    {
        m_pLogStream = new QTextStream(m_pLogFile);
        if ( m_pLogStream ) return true;
    }

    return false;
}

void LogWindow::CloseLogFile()
{
    KillLogStream();
    KillLogFile();
}

void LogWindow::KillLogStream()
{
    if ( m_pLogStream )
    {
        delete m_pLogStream;
        m_pLogStream = NULL;
    }
}

void LogWindow::KillLogFile()
{
    if ( m_pLogFile )
    {
        m_pLogFile->close();
        m_pLogFile = NULL;
    }
}

void LogWindow::show()
{
    if ( DEBUGGING ) QWidget::show();
}

void LogWindow::showAndRaise()
{
    if ( DEBUGGING )
    {
        show();
        raise();
    }
}

void LogWindow::printMessage(QString message)
{
    // Using insertPlainText as opposed to append since append automatically adds a newline
    // and we may not want this.
    m_pText->insertPlainText(message);
    m_pText->ensureCursorVisible();
}

void LogWindow::logMessage(QString message)
{
    if ( LOGGING && m_pLogFile && m_pLogStream )
    {
        m_pLogStream->operator << (message);
        m_pLogStream->flush();
    }
}

void LogWindow::printWarning(QString message)
{
    // Using insertPlainText as opposed to append since append automatically adds a newline
    // and we may not want this.
    QColor col = m_pText->textColor();
    int weight = m_pText->fontWeight();

    m_pText->setTextColor(QColor(255, 0, 0));
    m_pText->setFontWeight(LOG_WARNING_WEIGHT);

    m_pText->insertPlainText(message);
    m_pText->ensureCursorVisible();

    m_pText->setTextColor(col);
    m_pText->setFontWeight(weight);
}

void LogWindow::logWarning(QString message)
{
    if ( LOGGING && m_pLogFile && m_pLogStream )
    {
        m_pLogStream->operator << ("WARNING: ");
        m_pLogStream->operator << (message);
        m_pLogStream->flush();
    }
}

void LogWindow::zoomIn()
{
    m_pText->zoomIn(LOG_ZOOM_INCREMENT);
}

void LogWindow::zoomOut()
{
    m_pText->zoomOut(LOG_ZOOM_INCREMENT);
}

void LogWindow::newLogFile()
{
    CloseLogFile();

    QString date(__DATE__);
    date.remove(QChar(' '), Qt::CaseInsensitive);

    QString time(__TIME__);
    time.remove(QChar(':'), Qt::CaseInsensitive);

    QString filename(date + "_" + time + ".log");

    OpenLogFile(filename);
}

void LogWindow::newLogFile(QString filename)
{
    CloseLogFile();

    OpenLogFile(filename);
}
