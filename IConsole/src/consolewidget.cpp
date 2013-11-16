#include "consolewidget.h"
#include <QTextBlock>
#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QApplication>
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
    setCustomColor##_col(DEFAULT_CUSTOM_COLOUR##_col); \
}

const unsigned int ConsoleWidget::DEFAULT_MAX_LINES = 256;
const QColor ConsoleWidget::DEFAULT_MESSAGE_COLOUR = QColor(0,0,0);
const QColor ConsoleWidget::DEFAULT_WARNING_COLOUR = QColor(255,0,0);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR1 = QColor(208,208,208);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR2 = QColor(216,195,0);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR3 = QColor(255,106,43);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR4 = QColor(255,0,255);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR5 = QColor(0,255,255);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR6 = QColor(0,255,0);

// Implement custom colours
IMPLEMENT_CUSTOM_COLOUR(1)
IMPLEMENT_CUSTOM_COLOUR(2)
IMPLEMENT_CUSTOM_COLOUR(3)
IMPLEMENT_CUSTOM_COLOUR(4)
IMPLEMENT_CUSTOM_COLOUR(5)
IMPLEMENT_CUSTOM_COLOUR(6)

ConsoleWidget::ConsoleWidget(QWidget *parent) : QTextEdit(parent),
    m_iMaxLines(DEFAULT_MAX_LINES),
    m_colMessage(DEFAULT_MESSAGE_COLOUR),
    m_colWarning(DEFAULT_WARNING_COLOUR),
    m_colCustom1(DEFAULT_CUSTOM_COLOUR1),
    m_colCustom2(DEFAULT_CUSTOM_COLOUR2),
    m_colCustom3(DEFAULT_CUSTOM_COLOUR3),
    m_colCustom4(DEFAULT_CUSTOM_COLOUR4),
    m_colCustom5(DEFAULT_CUSTOM_COLOUR5),
    m_colCustom6(DEFAULT_CUSTOM_COLOUR6)
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
