#include "logwindow.h"
#include <QTextEdit>
#include <QBoxLayout>
#include "globals.h"
#include "commandlineparser.h"

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);  // We just want to hide when closed.
    //setWindowFlags(Qt::Tool);                 // This means the window does not show up on the taskbar - do we want this?

    setMinimumSize(320, 240);
    resize(640, 480);
    setWindowTitle("Log Output");

    m_pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_pLayout->setMargin(2);

    m_pText = new QTextEdit();
    m_pLayout->addWidget(m_pText);
    m_pText->setReadOnly(true);                     // User cannot enter text but it can be added programmatically.
    m_pText->setWordWrapMode(QTextOption::NoWrap);  // No wrapping by default.
    m_pText->setLineWrapMode(QTextEdit::NoWrap);
    m_pText->setUndoRedoEnabled(false);             // We don't want undo/redo to occur.
    m_pText->setFontFamily("Lucida Console");

    // Remove these when done testing (obviously).
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
    m_pText->append("This is a long text line. This text will show what happens when a line exceeds the width of the debug text area and no wrapping occurs.");
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
