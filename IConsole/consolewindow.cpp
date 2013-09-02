#include "consolewindow.h"

ConsoleWindow::ConsoleWindow(QWidget *parent) :
    QTextEdit(parent), m_colStd(0.0,0.0,0.0), m_colWarning(1.0,0.0,0.0)
{
    setReadOnly(true);
    setUndoRedoEnabled(false);
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
