#ifndef COMMANDSTORE_MANAGER_H
#define COMMANDSTORE_MANAGER_H

#include "commandstore_global.h"

class ListedCommandManager;
class ListedConsoleCommand;

namespace NCommandStore
{
    COMMANDSTORESHARED_EXPORT extern ListedCommandManager*    g_pCommandManager;
    COMMANDSTORESHARED_EXPORT extern ListedConsoleCommand*    g_pConsoleCommandList;
}

#endif // COMMANDSTORE_MANAGER_H
