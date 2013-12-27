#include "commandstore_manager.h"
#include "wr_listedcommandmanager.h"
#include "wr_listedconsolecommand.h"

namespace NCommandStore
{
    ListedConsoleCommand* g_pConsoleCommandList = NULL;
    ListedCommandManager* g_pCommandManager = new ListedCommandManager(g_pConsoleCommandList);
}
