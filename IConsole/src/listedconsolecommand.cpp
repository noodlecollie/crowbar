#include "wr_listedconsolecommand.h"

ListedConsoleCommand::ListedConsoleCommand(const QString &name, CommandManager *manager, ListedConsoleCommand **listHead,
                                           const QString &desc, NGlobalCmd::ArgCallback argCallback,
                                           NGlobalCmd::CMDFLAGS flags, QObject *parent) :
    BaseConsoleCommand(name, desc, argCallback, flags, parent)
{
    tryRegister(manager, listHead);
}

ListedConsoleCommand::ListedConsoleCommand(const QString &name, const QString &desc, NGlobalCmd::ArgCallback argCallback,
                                           NGlobalCmd::CMDFLAGS flags, QObject *parent) :
    BaseConsoleCommand(name, desc, argCallback, flags, parent)
{
}

ListedConsoleCommand* ListedConsoleCommand::getListNext() const
{
    return m_pListNext;
}

void ListedConsoleCommand::setListNext(ListedConsoleCommand *next)
{
    m_pListNext = next;
}

void ListedConsoleCommand::tryRegister(CommandManager *manager, ListedConsoleCommand **listHead)
{
    // If manager is not NULL, register.
    if ( manager )
    {
        manager->registerCommand(this);
        return;
    }
    
    // Otherwise, attach to list.
    setListNext(*listHead);
    *listHead = this;
}
