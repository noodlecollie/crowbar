#ifndef CONSOLECOMMANDSTORE_H
#define CONSOLECOMMANDSTORE_H

#include "concommand.h"
#include "convar.h"
#include "globals.h"
#include <QString>
#include <QStringList>

// ==================== Variables ====================

static const QString compileDate(__DATE__);
static const QString compileTime(__TIME__);
static ConVar g_compile_version("g_compile_version",
                                compileDate + " " + compileTime,
                                g_pCommandManager,
                                &g_pCommandList,
                                NULL,
                                "Date and time of last compile.",
                                NGlobalCmd::CMDFLAG_READONLY);

/**
 * @brief Callback for echo ConCommand.
 * @param info Information about the calling command manager.
 * @param args Arguments to this callback.
 * @param output Output variant.
 * @return Return code representing the success or failure of this callback's execution.
 */
int ccb_Echo(const CommandSenderInfo &info, const QStringList &args, QVariant &output);

//static DEFINE_CONCOMMAND( echo, ccb_Echo, "Echoes arguments to console and output variant.", 0 )
//static ConCommand echo("echo", ccb_Echo, g_pCommandManager, &g_pCommandList, "Echoes arguments to console and output variant.");

#endif // CONSOLECOMMANDSTORE_H
