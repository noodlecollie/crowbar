#include "consolewindow.h"

ConsoleWindow::ConsoleWindow(QWidget *parent) :
    QTextEdit(parent), m_colWarning(1.0,0.0,0.0), m_colStd(0.0,0.0,0.0)
{
    setReadOnly(true);
    setUndoRedoEnabled(false);
}


void ConsoleWindow::setWarningColour(const QColor &col)
{
    m_colWarning = col;
}

QColor ConsoleWindow::getWarningColour() const
{
    return m_colWarning;
}

void ConsoleWindow::setMessageColour(const QColor &col)
{
    m_colStd = col; setTextColor(col);
}

QColor ConsoleWindow::getMessageColour() const
{
    return m_colStd;
}

void ConsoleWindow::printWarning(const QString &message)
{
    setTextColor(m_colWarning);
    insertPlainText(message);
    setTextColor(m_colStd);
}

void ConsoleWindow::printMessage(const QString &message)
{
    setTextColor(m_colStd);
    insertPlainText(message);
}
