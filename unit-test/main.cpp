#include <QApplication>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "consolewidget.h"
#include "wr_commandentrybox.h"
#include "wr_listedcommandmanager.h"
#include "wr_convar.h"
#include "wr_concommand.h"
#include "wr_commandinterpreter.h"

int main(int, char**);
void init();
void myMessageOutput(QtMsgType, const QMessageLogContext &, const QString &);

void argCallback(QStringList &list, int /*argNum*/, const QString &/*current*/)
{
    list.append("A test string");
}

int testCallback(const CommandSenderInfo &info, const QStringList &/*args*/, QVariant &/*output*/)
{
    info.writeMessage("Message");
}

QWidget* mainWin = NULL;
QVBoxLayout* mainLayout = NULL;
QHBoxLayout* subLayout = NULL;
ConsoleWidget* consoleWindow = NULL;
CommandEntryBox* entryBox = NULL;
QPushButton* submitButton = NULL;
ListedCommandManager* commandManager = NULL;
ListedConsoleCommand* listHead = NULL;
CommandInterpreter* commandInterpreter = NULL;
ListedCommandManager* commandManager2 = NULL;

ConCommand testCommand("test_command", &testCallback, commandManager2, listHead, "Test command.", &argCallback);

ConVar test_var("test_var", "0", commandManager2, &listHead, NULL, "A test variable", NULL, 0, false, 0.0, false, 0.0);

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QDir dir(app.applicationDirPath());
    if ( dir.cd("resource") && dir.exists("manifest.qss") )
    {
        QFile stylesheet(dir.filePath("manifest.qss"));
        stylesheet.open(QFile::ReadOnly);
        QString setSheet = QLatin1String(stylesheet.readAll());
        app.setStyleSheet(setSheet);
    }
    
    init();
    mainWin->show();
    qDebug("Test");

    return app.exec();
}

void init()
{
    mainWin = new QWidget();
    mainLayout = new QVBoxLayout();
    subLayout = new QHBoxLayout();
    consoleWindow = new ConsoleWidget();
    entryBox = new CommandEntryBox();
    submitButton = new QPushButton("Submit");
    commandManager2 = new ListedCommandManager(listHead);
    commandManager = new ListedCommandManager(*commandManager2);
    commandInterpreter = new CommandInterpreter(commandManager);
    
    mainWin->setLayout(mainLayout);
    mainLayout->setMargin(2);
    mainLayout->addWidget(consoleWindow);
    mainLayout->addLayout(subLayout);
    subLayout->setMargin(0);
    subLayout->addWidget(entryBox);
    subLayout->addWidget(submitButton);
    
    // Submit button connections.
    submitButton->connect(submitButton, SIGNAL(clicked()), entryBox, SLOT(sendCommandString()));
    
    // Command entry box connections.
    entryBox->connect(entryBox, SIGNAL(commandString(QString)), commandInterpreter, SLOT(parse(QString)));
    entryBox->connect(entryBox, SIGNAL(getSuggestions(QString,QList<CommandInterpreter::CommandIdentPair>&,int)), commandInterpreter, SLOT(getSuggestions(QString,QList<CommandInterpreter::CommandIdentPair>&,int)));
    
    // Command interpreter connections.
    commandInterpreter->connect(commandInterpreter, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)), consoleWindow, SLOT(printMessage(CommandSenderInfo::OutputType,QString)));
    
    // Message handler.
    qInstallMessageHandler(myMessageOutput);
    //consoleWindow->printMessage("Test msg");
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    switch (type)
    {
        case QtDebugMsg:
        {
            if ( consoleWindow ) consoleWindow->printMessage(QString("]: %0\n").arg(msg));
            QTextStream(stdout) << msg << endl;
            break;
        }
        case QtWarningMsg:
        {
            if ( consoleWindow ) consoleWindow->printWarning(QString("]! %0\n").arg(msg));
            QTextStream(stderr) << msg << endl;
            break;
        }
        case QtCriticalMsg:
        {
            if ( consoleWindow ) consoleWindow->printWarning(QString("]!! %0\n").arg(msg));
            QTextStream(stderr) << msg << endl;
            break;
        }
        case QtFatalMsg:
        {
            if ( consoleWindow ) consoleWindow->printWarning(QString("]X %0\n").arg(msg));
            QTextStream(stderr) << msg << endl;
            abort();
        }
    }
}
