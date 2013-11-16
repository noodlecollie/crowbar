#include <QStringList>
#include <QVariant>

#include "globals.h"
#include "nglobalcmd.h"
#include "concommand.h"
#include "convar.h"
#include "listedcommandmanager.h"

int CB_con_testcolours(const CommandSenderInfo &info, const QStringList &/*args*/, QVariant &/*output*/)
{
    info.writeOutput(CommandSenderInfo::OutputGeneral, "Normal message.\n");
    info.writeOutput(CommandSenderInfo::OutputWarning, "Warning message.\n");
    info.writeOutput(CommandSenderInfo::OutputCustom1, "Custom colour 1 (inactive text).\n");
    info.writeOutput(CommandSenderInfo::OutputCustom2, "Custom colour 2 (highlighted text).\n");
    info.writeOutput(CommandSenderInfo::OutputCustom3, "Custom colour 3.\n");
    info.writeOutput(CommandSenderInfo::OutputCustom4, "Custom colour 4.\n");
    info.writeOutput(CommandSenderInfo::OutputCustom5, "Custom colour 5.\n");
    info.writeOutput(CommandSenderInfo::OutputCustom6, "Custom colour 6.\n");
    
    return NGlobalCmd::CCR_OK;
}
DEFINE_CONCOMMAND(con_testcolours, CB_con_testcolours, "Outputs test phrases for each of the 8 different supported console message colours", 0)
