#include "globals.h"

#include "convar.h"
#include "concommand.h"
#include "listedcommandmanager.h"   // ListedCommandManager is only forward-declared in globals.h so we need this here.
#include "commandstore.h"

DEFINE_CONCOMMAND(echo, CommandStore::ccb_Echo, "Echoes arguments to console and output variant.", 0)
DEFINE_CONCOMMAND(con_testcolours, CommandStore::ccb_Con_TestColours, "Outputs test phrases for each of the 8 different supported console message colours", 0)
