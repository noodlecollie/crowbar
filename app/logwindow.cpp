#include "logwindow.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QIcon>
#include <QToolBar>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include "globals.h"
#include "commandlineparser.h"
#include "consolewidget.h"

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent)
{
    // Set up window.
    setAttribute(Qt::WA_DeleteOnClose, false);  // We just want to hide when closed.
    //setWindowFlags(Qt::Tool);                 // This means the window does not show up on the taskbar - do we want this?

    //setMinimumSize(320, 240);
    resize(640, 480);
    setWindowTitle("Console");

    // Set up layout within window.
    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setMargin(2);

    // Text widget
    m_pText = new ConsoleWidget();
    m_pLayout->addWidget(m_pText);
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
    m_pText->printMessage(message);
}

void LogWindow::printWarning(QString message)
{
    m_pText->printWarning(message);
}
