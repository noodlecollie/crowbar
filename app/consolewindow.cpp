#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "consolewidget.h"
#include "commandentrybox.h"
#include "consolewindow.h"

using namespace NIConsole;

ConsoleWindow::ConsoleWindow(CommandInterpreter* interpreter, QWidget *parent) :
    QWidget(parent), m_pMainLayout(new QVBoxLayout()), m_pSubLayout(new QHBoxLayout()),
    m_pConsoleWidget(new ConsoleWidget()), m_pEntryBox(new CommandEntryBox()),
    m_pSubmitButton(new QPushButton("Submit"))
{
    init(interpreter);
}

void ConsoleWindow::init(CommandInterpreter *interpreter)
{
    setLayout(m_pMainLayout);
    m_pMainLayout->setMargin(2);
    m_pMainLayout->addWidget(m_pConsoleWidget);
    m_pMainLayout->addLayout(m_pSubLayout);
    m_pSubLayout->setMargin(0);
    m_pSubLayout->addWidget(m_pEntryBox);
    m_pSubLayout->addWidget(m_pSubmitButton);
    
    // Connect submit button to send the command string.
    m_pSubmitButton->connect(m_pSubmitButton, SIGNAL(clicked()), m_pEntryBox, SLOT(sendCommandString()));
    
    // Connect entry box command string signal to the passed command interpreter.
    m_pEntryBox->connect(m_pEntryBox, SIGNAL(commandString(QString)), interpreter, SLOT(parse(QString)));
    
    // Connect the entry box's suggestions request to the command interpreter.
    m_pEntryBox->connect(m_pEntryBox, SIGNAL(getSuggestions(QString,QList<CommandInterpreter::CommandIdentPair>&,int)), interpreter, SLOT(getSuggestions(QString,QList<CommandInterpreter::CommandIdentPair>&,int)));
    
    // Connect the interpreter's output message to print to the console window.
    interpreter->connect(interpreter, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)), m_pConsoleWidget, SLOT(printMessage(CommandSenderInfo::OutputType,QString)));
}

void ConsoleWindow::printToConsole(CommandSenderInfo::OutputType type, const QString &message)
{
    if ( m_pConsoleWidget )
    {
//        QColor col1 = m_pConsoleWidget->textColor();
//        QColor col2 = m_pConsoleWidget->messageColor();
//        m_pConsoleWidget->printMessage(type, QString("Colour: %0 %1 %2 %3 %4 %5\n").arg(col1.red()).arg(col1.green()).arg(col1.blue()).arg(col2.red()).arg(col2.green()).arg(col2.blue()));
        m_pConsoleWidget->printMessage(type, message);
    }
}

void ConsoleWindow::showAndRaise()
{
    show();
    raise();
    m_pEntryBox->setFocus(Qt::ActiveWindowFocusReason);
}
