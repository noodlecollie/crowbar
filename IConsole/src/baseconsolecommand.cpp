#include "baseconsolecommand.h"

BaseConsoleCommand::BaseConsoleCommand(const QString &name, const QString &desc, NGlobalCmd::ArgCallback argCallback, NGlobalCmd::CMDFLAGS flags, QObject *parent) :
    QObject(parent), m_szName(name), m_szDescription(desc), m_iFlags(flags), m_pArgCallback(argCallback)
{
}

NGlobalCmd::CmdIdent BaseConsoleCommand::identify() const
{
    return NGlobalCmd::CINull;
}

QString BaseConsoleCommand::name() const
{
    return m_szName;
}

QString BaseConsoleCommand::description() const
{
    return m_szDescription;
}

NGlobalCmd::CMDFLAGS BaseConsoleCommand::flagsRaw() const
{
    return m_iFlags;
}

void BaseConsoleCommand::setFlagsRaw(NGlobalCmd::CMDFLAGS flags)
{
    m_iFlags = flags;
}

void BaseConsoleCommand::setFlag(NGlobalCmd::CMDFLAGS flag)
{
    m_iFlags |= flag;
}

bool BaseConsoleCommand::flagSet(NGlobalCmd::CMDFLAGS flag) const
{
    return ( m_iFlags & flag ) == flag;
}

void BaseConsoleCommand::removeFlag(NGlobalCmd::CMDFLAGS flag)
{
    m_iFlags &= ~flag;
}

void BaseConsoleCommand::toggleFlag(NGlobalCmd::CMDFLAGS flag)
{
    m_iFlags ^= flag;
}

void BaseConsoleCommand::setArgSuggestionCallback(NGlobalCmd::ArgCallback callback)
{
    m_pArgCallback = callback;
}

NGlobalCmd::ArgCallback BaseConsoleCommand::argSuggestionCallback() const
{
    return m_pArgCallback;
}
