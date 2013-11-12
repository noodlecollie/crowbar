#include "consolewidget.h"
#include <QStyle>

// TEMP
#include <QtDebug>
// TEMP

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
const unsigned int ConsoleWidget::DEFAULT_MAX_LINES = 256;

// Implement our custom colours.
IMPLEMENT_CUSTOM_COLOUR(1)
IMPLEMENT_CUSTOM_COLOUR(2)
IMPLEMENT_CUSTOM_COLOUR(3)
IMPLEMENT_CUSTOM_COLOUR(4)
IMPLEMENT_CUSTOM_COLOUR(5)
IMPLEMENT_CUSTOM_COLOUR(6)

ConsoleWidget::ConsoleWidget(QWidget *parent) : QTextEdit(parent),
    m_colWarning(DEFAULT_WARNING_COLOUR),
    m_colMessage(DEFAULT_MESSAGE_COLOUR),
    m_colCustom1(DEFAULT_CUSTOM_COLOUR1),
    m_colCustom2(DEFAULT_CUSTOM_COLOUR2),
    m_colCustom3(DEFAULT_CUSTOM_COLOUR3),
    m_colCustom4(DEFAULT_CUSTOM_COLOUR4),
    m_colCustom5(DEFAULT_CUSTOM_COLOUR5),
    m_colCustom6(DEFAULT_CUSTOM_COLOUR6),
    m_iMaxLines(DEFAULT_MAX_LINES)
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
//    moveCursor(QTextCursor::End);
//    setTextColor(m_colWarning);
//    print(message);
//    setTextColor(m_colMessage);
    printMessage(CommandSenderInfo::OutputWarning, message);
}

void ConsoleWidget::printMessage(const QString &message)
{
    // BUG!! moveCursor resets text colour after moving, so it's here for now until this gets resolved.
//    moveCursor(QTextCursor::End);
//    setTextColor(m_colMessage);
//    print(message);
    printMessage(CommandSenderInfo::OutputGeneral, message);
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
    trimToLineLimit();
    ensureCursorVisible();
}

//void ConsoleWidget::printCustom(CommandSenderInfo::OutputType type, const QString &msg)
//{
//    // BUG!! moveCursor resets text colour after moving, so it's here for now until this gets resolved.
//    moveCursor(QTextCursor::End);
    
//    switch(type)
//    {
//        case CommandSenderInfo::OutputCustom1: setTextColor(m_colCustom1); break;
//        case CommandSenderInfo::OutputCustom2: setTextColor(m_colCustom2); break;
//        case CommandSenderInfo::OutputCustom3: setTextColor(m_colCustom3); break;
//        case CommandSenderInfo::OutputCustom4: setTextColor(m_colCustom4); break;
//        case CommandSenderInfo::OutputCustom5: setTextColor(m_colCustom5); break;
//        case CommandSenderInfo::OutputCustom6: setTextColor(m_colCustom6); break;
//        default:                               setTextColor(m_colMessage); break;
//    }
    
//    print(msg);
//}

void ConsoleWidget::printMessage(CommandSenderInfo::OutputType type, const QString &message)
{
//    QColor col1 = textColor();
//    QColor col2 = messageColor();
//    qDebug() << col1.red() << col1.green() << col1.blue() << col2.red() << col2.green() << col2.blue();
    
    switch(type)
    {
        case CommandSenderInfo::OutputWarning: setTextColor(m_colWarning); break;
        case CommandSenderInfo::OutputCustom1: setTextColor(m_colCustom1); break;
        case CommandSenderInfo::OutputCustom2: setTextColor(m_colCustom2); break;
        case CommandSenderInfo::OutputCustom3: setTextColor(m_colCustom3); break;
        case CommandSenderInfo::OutputCustom4: setTextColor(m_colCustom4); break;
        case CommandSenderInfo::OutputCustom5: setTextColor(m_colCustom5); break;
        case CommandSenderInfo::OutputCustom6: setTextColor(m_colCustom6); break;
        default:                               setTextColor(m_colMessage); break;
    }
    
    print(message);
    
//    qDebug() << "After:" << col1.red() << col1.green() << col1.blue() << col2.red() << col2.green() << col2.blue();
}

int ConsoleWidget::maxLines()
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
