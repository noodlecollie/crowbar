#include "wr_listedcommandmanager.h"

ListedCommandManager::ListedCommandManager(ListedConsoleCommand * listHead, QObject *parent) :
    CommandManager(parent)
{
    traverse(listHead);
}

ListedCommandManager::ListedCommandManager(QObject *parent) : CommandManager(parent)
{
}

void ListedCommandManager::traverse(ListedConsoleCommand * listHead)
{
    // NOTE: Changed this function sig so the pointer is no longer const.
    // This means that it will always be left NULL at the end of the process.
    // Traverse the list while it's not null.
    while ( listHead )
    {
        registerCommand(listHead);
        listHead = listHead->getListNext();
    }
}
