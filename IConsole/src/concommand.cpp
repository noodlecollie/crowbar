#include "wr_concommand.h"

ConCommand::ConCommand(const QString &name, NGlobalCmd::CmdCallback callback, const QString &desc, NGlobalCmd::CMDFLAGS flags, QObject *parent) :
    ListedConsoleCommand(name, desc, flags, parent), m_pCallback(callback)
{
}

ConCommand::ConCommand(const QString &name, NGlobalCmd::CmdCallback callback, CommandManager *manager, ListedConsoleCommand **list,
                       const QString &desc, const NGlobalCmd::CMDFLAGS flags, QObject *parent) :
    ListedConsoleCommand(name, manager, list, desc, flags, parent), m_pCallback(callback)
{
}

int ConCommand::exec(const QString &cmd, const QStringList &args, QVariant &output)
{
    // If we have no callback, return this error.
        if ( m_pCallback == NULL ) return NGlobalCmd::CCR_NoCallback;
    
    // Otherwise return the results of the callback.
    return (m_pCallback)(cmd, args, output);
}

NGlobalCmd::CmdCallback ConCommand::getExec() const
{
    return m_pCallback;
}

void ConCommand::setExec(NGlobalCmd::CmdCallback cmd)
{
    m_pCallback = cmd;
}

NGlobalCmd::CmdIdent ConCommand::identify() const
{
    return NGlobalCmd::CICommand;
}
