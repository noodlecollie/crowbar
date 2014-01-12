#ifndef LOCALCOMMANDS_H
#define LOCALCOMMANDS_H

#include "commandsenderinfo.h"
#include "wr_concommand.h"
#include "wr_commandmanager.h"
#include "wr_listedcommandmanager.h"
#include "commandstore_manager.h"

int ccb_DumpGlobalStyleSheet(const NIConsole::CommandSenderInfo &info, const QStringList &args, QVariant &output);
static NIConsole::ConCommand dev_dump_style_sheet("dev_dump_style_sheet", ccb_DumpGlobalStyleSheet, NCommandStore::g_pCommandManager,
                                                  &NCommandStore::g_pConsoleCommandList,
                                                  "Dumps the global style sheet to the console.");

int ccb_RefreshStyleSheet(const NIConsole::CommandSenderInfo &info, const QStringList &args, QVariant &output);
static NIConsole::ConCommand dev_refresh_style_sheet("dev_refresh_style_sheet", ccb_RefreshStyleSheet, NCommandStore::g_pCommandManager,
                                                     &NCommandStore::g_pConsoleCommandList,
                                                     "Refreshes the global style sheet.");

static NIConsole::ConVar gl_max_version("gl_max_version", "0.0", NCommandStore::g_pCommandManager, &NCommandStore::g_pConsoleCommandList, NULL,
                                        "Read-only. Maximum OpenGL version available on the system.", NGlobalCmd::CMDFLAG_READONLY, true, 0.0);

// Returns true if style sheet was successfully applied.
bool applyStyleSheet();

#endif // LOCALCOMMANDS_H
