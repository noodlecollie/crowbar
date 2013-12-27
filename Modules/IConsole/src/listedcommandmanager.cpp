#include "wr_listedcommandmanager.h"

ListedCommandManager::ListedCommandManager(ListedConsoleCommand * listHead, QObject *parent) :
    CommandManager(parent)
{
    traverse(listHead);
}

ListedCommandManager::ListedCommandManager(ListedCommandManager &other, QObject *parent) :
    CommandManager(other, parent)
{
}

ListedCommandManager::ListedCommandManager(QObject *parent) : CommandManager(parent)
{
}

void ListedCommandManager::traverse(ListedConsoleCommand * listHead)
{
    //qDebug("Traversing list head for %p", listHead);
    // NOTE: Changed this function sig so the pointer is no longer const.
    // This means that it will always be left NULL at the end of the process.
    // Traverse the list while it's not null.
    while ( listHead )
    {
        //qDebug("Registering command: %s", listHead->name().toLatin1().constData());
        registerCommand(listHead);
        listHead = listHead->getListNext();
    }
}
