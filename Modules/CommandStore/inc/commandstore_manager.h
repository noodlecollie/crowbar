/** \file commandstore_manager.h
 * \brief Exposes the global command manager and console command list.
 */

#ifndef COMMANDSTORE_MANAGER_H
#define COMMANDSTORE_MANAGER_H

#include "commandstore_global.h"
#include "iconsole_global.h"

ICONSOLE_BEGIN_NAMESPACE
class ListedCommandManager;
class ListedConsoleCommand;
ICONSOLE_END_NAMESPACE

COMMANDSTORE_BEGIN_NAMESPACE

/**
 * @brief Global command manager pointer. Is NULL if manager has not been created yet.
 */
COMMANDSTORESHARED_EXPORT extern NIConsole::ListedCommandManager*   g_pCommandManager;

/**
 * @brief Global console command list. If NULL, no console commands are waiting to be added to the manager.
 */
COMMANDSTORESHARED_EXPORT extern NIConsole::ListedConsoleCommand*   g_pConsoleCommandList;
    
COMMANDSTORE_END_NAMESPACE

#endif // COMMANDSTORE_MANAGER_H
