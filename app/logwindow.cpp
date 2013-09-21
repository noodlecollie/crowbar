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

    // Text widget
    m_pText = new QTextEdit();
    m_pLayout->addWidget(m_pText);
    m_pText->setReadOnly(true);                     // User cannot enter text but it can be added programmatically.
    m_pText->setWordWrapMode(QTextOption::WordWrap);
    m_pText->setLineWrapMode(QTextEdit::WidgetWidth);
    m_pText->setUndoRedoEnabled(false);             // We don't want undo/redo to occur.
    m_pText->setFontFamily("Lucida Console");
}

LogWindow::~LogWindow()
{
}

void LogWindow::show()
{
    QWidget::show();
}

void LogWindow::showAndRaise()
{
    show();
    raise();
}

void LogWindow::printMessage(QString message)
{
    // Using insertPlainText as opposed to append since append automatically adds a newline
    // and we may not want this.
    m_pText->insertPlainText(message);
    m_pText->ensureCursorVisible();
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
