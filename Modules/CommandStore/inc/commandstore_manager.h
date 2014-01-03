#ifndef COMMANDSTORE_MANAGER_H
#define COMMANDSTORE_MANAGER_H

#include "commandstore_global.h"
#include "iconsole_global.h"

ICONSOLE_BEGIN_NAMESPACE
class ListedCommandManager;
class ListedConsoleCommand;
ICONSOLE_END_NAMESPACE

COMMANDSTORE_BEGIN_NAMESPACE

COMMANDSTORESHARED_EXPORT extern NIConsole::ListedCommandManager*   g_pCommandManager;
COMMANDSTORESHARED_EXPORT extern NIConsole::ListedConsoleCommand*   g_pConsoleCommandList;
    
COMMANDSTORE_END_NAMESPACE

#endif // COMMANDSTORE_MANAGER_H
