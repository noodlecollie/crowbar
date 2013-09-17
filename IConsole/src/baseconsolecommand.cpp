#include "baseconsolecommand.h"

BaseConsoleCommand::BaseConsoleCommand(const QString &name, const QString &desc, NGlobalCmd::CMDFLAGS flags, QObject *parent) :
    QObject(parent), m_szName(name), m_szDescription(desc), m_iFlags(flags)
{
}

NGlobalCmd::CmdIdent BaseConsoleCommand::identify() const
{
    return NGlobalCmd::CINull;
}

const QString& BaseConsoleCommand::getName() const
{
    return m_szName;
}

const QString& BaseConsoleCommand::getDescription() const
{
    return m_szDescription;
}

NGlobalCmd::CMDFLAGS BaseConsoleCommand::getFlags() const
{
    return m_iFlags;
}

void BaseConsoleCommand::setFlags(NGlobalCmd::CMDFLAGS flags)
{
    m_iFlags = flags;
}
