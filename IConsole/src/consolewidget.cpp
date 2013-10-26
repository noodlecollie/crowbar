#include "consolewidget.h"
#include <QStyle>

#define IMPLEMENT_CUSTOM_COLOUR(_col) \
QColor ConsoleWidget::customColor##_col() const \
{ \
    return m_colCustom##_col; \
} \
void ConsoleWidget::setCustomColor##_col(QColor col) \
{ \
    m_colCustom##_col = col; \
} \
void ConsoleWidget::resetCustomColor##_col() \
{ \
    m_colCustom##_col = DEFAULT_CUSTOM_COLOUR##_col; \
}

const QColor ConsoleWidget::DEFAULT_WARNING_COLOUR = QColor(255,0,0);
const QColor ConsoleWidget::DEFAULT_MESSAGE_COLOUR = QColor(0,0,0);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR1 = QColor(51,181,48);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR2 = QColor(58,107,255);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR3 = QColor(255,106,43);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR4 = QColor(206,206,206);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR5 = QColor(0,255,255);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR6 = QColor(255,0,255);

// Implement our custom colours.
IMPLEMENT_CUSTOM_COLOUR(1)
IMPLEMENT_CUSTOM_COLOUR(2)
IMPLEMENT_CUSTOM_COLOUR(3)
IMPLEMENT_CUSTOM_COLOUR(4)
IMPLEMENT_CUSTOM_COLOUR(5)
IMPLEMENT_CUSTOM_COLOUR(6)

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

void ConsoleWidget::printCustom(CommandSenderInfo::OutputType type, const QString &msg)
{
    // BUG!! moveCursor resets text colour after moving, so it's here for now until this gets resolved.
    moveCursor(QTextCursor::End);
    
    switch(type)
    {
        case CommandSenderInfo::OutputCustom1: setTextColor(m_colCustom1); break;
        case CommandSenderInfo::OutputCustom2: setTextColor(m_colCustom2); break;
        case CommandSenderInfo::OutputCustom3: setTextColor(m_colCustom3); break;
        case CommandSenderInfo::OutputCustom4: setTextColor(m_colCustom4); break;
        case CommandSenderInfo::OutputCustom5: setTextColor(m_colCustom5); break;
        case CommandSenderInfo::OutputCustom6: setTextColor(m_colCustom6); break;
        default:                               setTextColor(m_colMessage); break;
    }
    
    print(msg);
}

void ConsoleWidget::printMessage(CommandSenderInfo::OutputType type, const QString &message)
{
    switch (type)
    {
        case CommandSenderInfo::OutputWarning: printWarning(message); break;
        case CommandSenderInfo::OutputCustom1:
        case CommandSenderInfo::OutputCustom2:
        case CommandSenderInfo::OutputCustom3:
        case CommandSenderInfo::OutputCustom4:
        case CommandSenderInfo::OutputCustom5:
        case CommandSenderInfo::OutputCustom6: printCustom(type, message); break;
        default:                               printMessage(message); break;
    }
}
