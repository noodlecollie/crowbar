#include "commandsenderinfo.h"
#include "wr_commandmanager.h"

ICONSOLE_BEGIN_NAMESPACE

CommandSenderInfo::CommandSenderInfo(QString name, CommandManager *manager, SendOutput outputFunc,
                                     bool hasMin, float min, bool hasMax, float max, QObject *parent) :
    QObject(parent), m_pManager(manager), m_pOutput(outputFunc), m_NameOfCommand(name), m_bHasMin(hasMin),
    m_flMinValue(min), m_bHasMax(hasMax), m_flMaxValue(max)
{
}

CommandSenderInfo::CommandSenderInfo(const CommandSenderInfo &other) :
    QObject(NULL), m_pManager(NULL), m_pOutput(NULL), m_NameOfCommand(""), m_bHasMin(false),
    m_flMinValue(0.0f), m_bHasMax(false), m_flMaxValue(0.0f)
{
    setParent(other.parent());
    
    // We can copy these over directly because other CommandSenderInfo classes are friends.
    // We need to do this because commandManager() returns a const pointer, which we can't use here.
    m_pManager = other.m_pManager;
    m_pOutput = other.m_pOutput;
    m_NameOfCommand = other.m_NameOfCommand;
    m_bHasMin = other.m_bHasMin;
    m_bHasMax = other.m_bHasMax;
    m_flMinValue = other.m_flMinValue;
    m_flMaxValue = other.m_flMaxValue;
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

CommandSenderInfo::SendOutput CommandSenderInfo::outputCallback() const
{
    return m_pOutput;
}

void CommandSenderInfo::setOutputCallback(CommandSenderInfo::SendOutput output)
{
    m_pOutput = output;
}

QString CommandSenderInfo::commandName() const
{
    return m_NameOfCommand;
}

void CommandSenderInfo::setCommandName(QString &name)
{
    m_NameOfCommand = name;
}

CommandManager* CommandSenderInfo::commandManager() const
{
    return m_pManager;
}

void CommandSenderInfo::setCommandManager(CommandManager *manager)
{
    m_pManager = manager;
}

bool CommandSenderInfo::hasMin() const
{
    return m_bHasMin;
}

void CommandSenderInfo::setHasMin(bool b)
{
    m_bHasMin = b;
}

bool CommandSenderInfo::hasMax() const
{
    return m_bHasMax;
}

void CommandSenderInfo::setHasMax(bool b)
{
    m_bHasMax = b;
}

float CommandSenderInfo::minValue() const
{
    return m_flMinValue;
}

void CommandSenderInfo::setMinValue(float val)
{
    m_flMinValue = val;
}

float CommandSenderInfo::maxValue() const
{
    return m_flMaxValue;
}

void CommandSenderInfo::setMaxValue(float val)
{
    m_flMaxValue = val;
}

ICONSOLE_END_NAMESPACE
