#include "consolewidget.h"
#include <QStyle>

const QColor ConsoleWidget::DEFAULT_WARNING_COLOUR = QColor(255,0,0);
const QColor ConsoleWidget::DEFAULT_MESSAGE_COLOUR = QColor(0,0,0);

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QTextEdit(parent), m_colWarning(DEFAULT_WARNING_COLOUR), m_colMessage(DEFAULT_MESSAGE_COLOUR)
{
    setReadOnly(true);
    setUndoRedoEnabled(false);
    setWordWrapMode(QTextOption::WrapAnywhere);
    setLineWrapMode(QTextEdit::WidgetWidth);
}


void ConsoleWidget::setWarningColor(QColor warningColor)
{
    m_colWarning = warningColor;
}

QColor ConsoleWidget::warningColor() const
{
    return m_colWarning;
}

void ConsoleWidget::resetWarningColor()
{
    m_colWarning = DEFAULT_WARNING_COLOUR;
}

QColor ConsoleWidget::messageColor() const
{
   return m_colMessage; 
}

void ConsoleWidget::setMessageColor(QColor messageColor)
{
    m_colMessage = messageColor;
}

void ConsoleWidget::resetMessageColor()
{
    m_colMessage = DEFAULT_MESSAGE_COLOUR;
}

void ConsoleWidget::printWarning(const QString &message)
{
    // BUG!! moveCursor resets text colour after moving, so it's here for now until this gets resolved.
    moveCursor(QTextCursor::End);
    setTextColor(m_colWarning);
    print(message);
    setTextColor(m_colMessage);
}

void ConsoleWidget::printMessage(const QString &message)
{
    // BUG!! moveCursor resets text colour after moving, so it's here for now until this gets resolved.
    moveCursor(QTextCursor::End);
    setTextColor(m_colMessage);
    print(message);
}

void ConsoleWidget::print(const QString &msg)
{
    // Using insertPlainText as opposed to append since append automatically adds a newline
    // and we may not want this.
    
    // BUG!! moveCursor resets the text colour after execution.
    // To demonstrate, uncomment the lines below.
    // moveCursor has been moved to printMessage and printWarning so that they can set the respective text colour manually after moving.
//    qDebug("Before: %d %d %d", textColor().red(), textColor().green(), textColor().blue());
//    moveCursor(QTextCursor::End);
//    qDebug("After: %d %d %d", textColor().red(), textColor().green(), textColor().blue());
    
    insertPlainText(msg);
    ensureCursorVisible();
}

void ConsoleWidget::printMessage(CommandSenderInfo::OutputType type, const QString &message)
{
    switch (type)
    {
        case CommandSenderInfo::OutputWarning:
        {
            printWarning(message);
            break;
        }
        default:
        {
            printMessage(message);
            break;
        }
    }
}
