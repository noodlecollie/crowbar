#include "consolewidget.h"

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QTextEdit(parent), m_colWarning(1.0,0.0,0.0), m_colStd(0.0,0.0,0.0)
{
    setReadOnly(true);
    setUndoRedoEnabled(false);
    setWordWrapMode(QTextOption::WordWrap);
    setLineWrapMode(QTextEdit::WidgetWidth);
}


void ConsoleWidget::setWarningColour(const QColor &col)
{
    m_colWarning = col;
}

QColor ConsoleWidget::getWarningColour() const
{
    return m_colWarning;
}

void ConsoleWidget::setMessageColour(const QColor &col)
{
    m_colStd = col; setTextColor(col);
}

QColor ConsoleWidget::getMessageColour() const
{
    return m_colStd;
}

void ConsoleWidget::printWarning(const QString &message)
{
    setTextColor(m_colWarning);
    printMessage(message);
    setTextColor(m_colStd);
}

void ConsoleWidget::printMessage(const QString &message)
{
    // Using insertPlainText as opposed to append since append automatically adds a newline
    // and we may not want this.
    insertPlainText(message);
    ensureCursorVisible();
}
