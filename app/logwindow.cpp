#include "logwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColor>
#include <QPushButton>
#include <QPoint>
#include <QResizeEvent>
#include <QMoveEvent>
#include "globals.h"
#include "commandlineparser.h"
#include "consolewidget.h"
#include "wr_commandentrybox.h"
#include "commandsuggestionlist.h"
#include "wr_commandinterpreter.h"

LogWindow::LogWindow(CommandInterpreter* interpreter, QWidget *parent) :
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
    m_pText->setFocusPolicy(Qt::NoFocus);
    
    // Bottom layout.
    m_pBottomLayout = new QHBoxLayout();
    m_pLayout->addLayout(m_pBottomLayout);
    
    // Entry box.
    m_pEntry = new CommandEntryBox();
    m_pBottomLayout->addWidget(m_pEntry);
    m_pEntry->setFocusPolicy(Qt::NoFocus);
    m_pEntry->connect(m_pEntry, SIGNAL(commandString(QString)), interpreter, SLOT(parse(QString)));
    m_pEntry->connect(m_pEntry, SIGNAL(getSuggestions(QString,QList<CommandInterpreter::CommandIdentPair>&,int)), interpreter, SLOT(getSuggestions(QString,QList<CommandInterpreter::CommandIdentPair>&,int)));
    
    // Exec button.
    m_pExecButton = new QPushButton("Submit");
    m_pBottomLayout->addWidget(m_pExecButton);
    m_pExecButton->setFocusPolicy(Qt::NoFocus);
    m_pExecButton->connect(m_pExecButton, SIGNAL(clicked()), m_pEntry, SLOT(sendCommandString()));
    
    // Suggestion list.
    m_pSgList = new CommandSuggestionList(this);
    m_pSgList->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool /*| Qt::WindowStaysOnTopHint*/);
    m_pSgList->setMaximumHeight(200);
    m_pSgList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pSgList->setUniformItemSizes(true);
    m_pSgList->hide();
    m_pEntry->setSuggestionList(m_pSgList);
}

LogWindow::~LogWindow()
{
    if ( m_pSgList ) delete m_pSgList;
}

void LogWindow::showAndRaise()
{
    show();
    raise();
}

void LogWindow::moveEvent(QMoveEvent *e)
{
    // Base event first.
    QWidget::moveEvent(e);
    
    // Tell the entry box to reposition its suggestions list.
    m_pEntry->repositionSuggestions();
}

void LogWindow::showEvent(QShowEvent *e)
{
    // Base event first.
    QWidget::showEvent(e);
    
    // Set focus to the entry box.
    if ( m_pEntry ) m_pEntry->setFocus(Qt::OtherFocusReason);
}

//void LogWindow::printMessage(QString message)
//{
//    m_pText->printMessage(message);
//}

//void LogWindow::printWarning(QString message)
//{
//    m_pText->printWarning(message);
//}

void LogWindow::printMessage(CommandSenderInfo::OutputType type, const QString &message)
{
    switch (type)
    {
        case CommandSenderInfo::OutputWarning:
        {
            m_pText->printWarning(message);
            break;
        }
        default:
        {
            m_pText->printMessage(message);
            break;
        }
    }
}
