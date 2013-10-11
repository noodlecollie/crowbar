#include "commandsenderinfo.h"
#include "wr_commandmanager.h"

CommandSenderInfo::CommandSenderInfo(QString name, CommandManager *manager, SendOutput outputFunc, QObject *parent) :
    QObject(parent), m_pManager(manager), m_pOutput(outputFunc), m_NameOfCommand(name)
{
}

void CommandSenderInfo::writeOutput(CommandSenderInfo::OutputType type, const QString &output) const
{
    // Call the function.
    if ( m_pManager && m_pOutput ) (m_pManager->*m_pOutput)(type, output);
}

void CommandSenderInfo::writeMessage(const QString &message) const
{
    writeOutput(OutputGeneral, message);
}

void CommandSenderInfo::writeWarning(const QString &message) const
{
    writeOutput(OutputWarning, message);
}

CommandSenderInfo::SendOutput CommandSenderInfo::getOutputPointer() const
{
    return m_pOutput;
}

void CommandSenderInfo::setOutputPointer(CommandSenderInfo::SendOutput output)
{
    m_pOutput = output;
}

QString CommandSenderInfo::getName() const
{
    return m_NameOfCommand;
}

void CommandSenderInfo::setName(QString &name)
{
    m_NameOfCommand = name;
}

const CommandManager* CommandSenderInfo::getManager() const
{
    return m_pManager;
}

void CommandSenderInfo::setManager(CommandManager *manager)
{
    m_pManager = manager;
}
