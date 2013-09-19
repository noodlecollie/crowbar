#include "wr_listedcommandmanager.h"

ListedCommandManager::ListedCommandManager(ListedConsoleCommand * const listHead, QObject *parent) :
    CommandManager(parent)
{
    traverse(listHead);
}

ListedCommandManager::ListedCommandManager(QObject *parent) : CommandManager(parent)
{
}

void ListedCommandManager::traverse(ListedConsoleCommand * const listHead)
{
    ListedConsoleCommand* list = listHead;
    
    // Traverse the list while it's not null.
    while ( list )
    {
        registerCommand(list);
        list = list->getListNext();
    }
}
