#include "consolewidget.h"
#include <QTextBlock>
#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QStyle>

const unsigned int ConsoleWidget::DEFAULT_MAX_LINES = 256;
const QColor ConsoleWidget::DEFAULT_MESSAGE_COLOUR = QColor(0,0,0);
const QColor ConsoleWidget::DEFAULT_WARNING_COLOUR = QColor(255,0,0);

ConsoleWidget::ConsoleWidget(QWidget *parent) : QTextEdit(parent),
    m_iMaxLines(DEFAULT_MAX_LINES),
    m_colMessage(DEFAULT_MESSAGE_COLOUR),
    m_colWarning(DEFAULT_WARNING_COLOUR)
{
    setReadOnly(true);
    setUndoRedoEnabled(false);
    setWordWrapMode(QTextOption::WrapAnywhere);
    setLineWrapMode(QTextEdit::WidgetWidth);
    
    // IT TOOK ME SO LONG TO FIND OUT THIS SOLUTION.
    //Un-polish and re-polish to ensure any QProperties set via style sheet are updated.
    repolish();
}

void ConsoleWidget::printWarning(const QString &message)
{
    printMessage(CommandSenderInfo::OutputWarning, message);
}

void ConsoleWidget::printMessage(const QString &message)
{
    printMessage(CommandSenderInfo::OutputGeneral, message);
}

void ConsoleWidget::printMessage(CommandSenderInfo::OutputType type, const QString &message)
{
    moveCursor(QTextCursor::End);
    
    if ( type == CommandSenderInfo::OutputWarning )
    {
        setTextColor(m_colWarning);
        print(message);
    }
    else
    {
        setTextColor(m_colMessage);
        print(message);
    }
}

void ConsoleWidget::print(const QString &msg)
{
    insertPlainText(msg);
    
    trimToLineLimit();
    ensureCursorVisible();
}

int ConsoleWidget::maxLines() const
{
    return m_iMaxLines;
}

void ConsoleWidget::setMaxLines(int lines)
{
    m_iMaxLines = lines;
    trimToLineLimit();
}

void ConsoleWidget::resetMaxLines()
{
    setMaxLines(DEFAULT_MAX_LINES);
}

void ConsoleWidget::trimToLineLimit()
{
    // When the max block limit is set on the underlying QTextDocument, any lines exceeding the limit will be
    // trimmed from the top of the document.
    
    QTextDocument* doc = document();
    if ( doc && m_iMaxLines >= 1 && doc->blockCount() > m_iMaxLines )   // No point if m_iMaxLines is 0, since blockCount should never be 0.
    {
        doc->setMaximumBlockCount(m_iMaxLines);
        doc->setMaximumBlockCount(0);
    }
}

QColor ConsoleWidget::messageColor() const
{
    return m_colMessage;
}

void ConsoleWidget::setMessageColor(QColor col)
{
    m_colMessage = col;
}

void ConsoleWidget::resetMessageColor()
{
    setMessageColor(DEFAULT_MESSAGE_COLOUR);
}

QColor ConsoleWidget::warningColor() const
{
    return m_colWarning;
}

void ConsoleWidget::setWarningColor(QColor col)
{
    m_colWarning = col;
}

void ConsoleWidget::resetWarningColor()
{
    setMessageColor(DEFAULT_WARNING_COLOUR);
}

void ConsoleWidget::repolish()
{
    style()->unpolish(this);
    style()->polish(this);
}
