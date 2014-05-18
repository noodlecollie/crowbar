#include "commandstore_manager.h"
#include "wr_listedcommandmanager.h"
#include "wr_listedconsolecommand.h"

COMMANDSTORE_BEGIN_NAMESPACE

using namespace ICONSOLE_NAMESPACE;

ListedConsoleCommand* g_pConsoleCommandList = NULL;

// Pass a blank string as the base directory for now - we'll update this later.
ListedCommandManager* g_pCommandManager = new ListedCommandManager(QString(""), g_pConsoleCommandList);

COMMANDSTORE_END_NAMESPACE
