#ifndef LISTEDCOMMANDMANAGER_H
#define LISTEDCOMMANDMANAGER_H

#include "iconsole_global.h"
#include "commandmanager.h"

class ListedConsoleCommand;

class ICONSOLESHARED_EXPORT ListedCommandManager : public CommandManager
{
public:
    explicit ListedCommandManager(QObject* parent = 0);
    explicit ListedCommandManager(ListedConsoleCommand* const listHead, QObject* parent = 0);
    
    void traverse(ListedConsoleCommand* const listHead);
};

#endif // LISTEDCOMMANDMANAGER_H
