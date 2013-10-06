#ifndef LISTEDCOMMANDMANAGER_H
#define LISTEDCOMMANDMANAGER_H

#include "iconsole_global.h"
#include "commandmanager.h"

class ListedConsoleCommand;

class ICONSOLESHARED_EXPORT ListedCommandManager : public CommandManager
{
public:
    explicit ListedCommandManager(QObject* parent = 0);
    explicit ListedCommandManager(ListedConsoleCommand* listHead, QObject* parent = 0);
    virtual ~ListedCommandManager() {}
    
    void traverse(ListedConsoleCommand* listHead);
};

#endif // LISTEDCOMMANDMANAGER_H
