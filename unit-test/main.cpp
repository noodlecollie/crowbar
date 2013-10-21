#include <QApplication>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QDir>
#include "consolewidget.h"
#include "wr_commandentrybox.h"
#include "wr_listedcommandmanager.h"
#include "wr_convar.h"
#include "wr_concommand.h"
#include "wr_commandinterpreter.h"

int main(int, char**);
void init();
void myMessageOutput(QtMsgType, const QMessageLogContext &, const QString &);


QWidget* mainWin = NULL;
QVBoxLayout* mainLayout = NULL;
QHBoxLayout* subLayout = NULL;
ConsoleWidget* consoleWindow = NULL;
CommandEntryBox* entryBox = NULL;
QPushButton* submitButton = NULL;
ListedCommandManager* commandManager = NULL;
ListedConsoleCommand* listHead = NULL;
CommandInterpreter* commandInterpreter = NULL;

ConVar test_var("test_var", "0", commandManager, &listHead, NULL, "A test variable", 0, false, 0.0, false, 0.0);

int aCallback(const CommandSenderInfo &info, const QStringList &args, QVariant &output)
{
    info.writeMessage("Arguments we were passed:\n");
    
    foreach(QString s, args)
    {
        info.writeMessage(s + QString("\n"));
    }
    
    output.setValue(QString("Yiff yiff acid bath"));
    return NGlobalCmd::CCR_OK;
}
ConCommand exampleCmd("example_cmd", &aCallback, commandManager, &listHead, "An example command");

int callback_echo(const CommandSenderInfo &info, const QStringList &args, QVariant &output)
{
    QString message;
    bool first = true;
    foreach(QString s, args)
    {
        message.append(QString((first ? "" : " ") + s));
        first = false;
    }
    info.writeMessage(message + QString("\n"));
    
    output.setValue(message);
    
    return NGlobalCmd::CCR_OK;
}
ConCommand echo("echo", &callback_echo, commandManager, &listHead, "Echos arguments to output.");

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
    commandManager = new ListedCommandManager(listHead);
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
    //qInstallMessageHandler(myMessageOutput);
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    switch (type)
    {
    case QtDebugMsg:
        if ( consoleWindow ) consoleWindow->printMessage(QString("QTDEBUG: %0\n").arg(msg));
        break;
    case QtWarningMsg:
        if ( consoleWindow ) consoleWindow->printWarning(QString("QTDEBUG WARNING: %0\n").arg(msg));
        break;
    case QtCriticalMsg:
        if ( consoleWindow ) consoleWindow->printWarning(QString("QTDEBUG CRITICAL: %0\n").arg(msg));
        break;
    case QtFatalMsg:
        if ( consoleWindow ) consoleWindow->printWarning(QString("QTDEBUG FATAL: %0\n").arg(msg));
        abort();
    }
}
