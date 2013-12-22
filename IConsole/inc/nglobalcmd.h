/** \file nglobalcmd.h
 * \brief Defines global properties for console commands.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef NGLOBALCMD_H
#define NGLOBALCMD_H

#include <QString>
#include <QVariant>
#include "commandsenderinfo.h"

// Global command properties.
namespace NGlobalCmd
{
    // Identifier for the type of a command.
    /**
     * @brief Command type identifiers.
     */
    enum CmdIdent
    {
        CINull = 0, /**< Null (BaseConsoleCommand) */
        CICommand,  /**< Command (ConCommand) */
        CIVariable  /**< Variable (ConVar) */
    };
    
    /**
     * @brief Return types for ConCommands.
     */
    enum ConCommandReturn
    {
        CCR_OK = 0,     /**< Command executed OK. */
        CCR_NoCallback  /**< The command had no callback. */
    };
    
    // Command flags
    /**
     * @brief Typedef for command flags.
     */
    typedef unsigned int CMDFLAGS;
    
    /**
     * @brief Command flag values.
     */
    enum
    {
        CMDFLAG_READONLY = 0x1,         /**< ConVar's value cannot be changed. */
        CMDFLAG_ENSURECALLBACK = 0x2    /**< ConVar or ConCommand must have a callback. */
    };  
    
    /**
     * @brief Typedef for a ConCommand callback.
     */
    typedef int (*CmdCallback) (const CommandSenderInfo &info, const QStringList &args, QVariant &output);
    
    /**
     * @brief Typedef for a ConVar callback.
     */
    typedef void (*VarCallback) (const CommandSenderInfo &info, const QString &oldValue, QString &newValue);
}

#endif // NGLOBALCMD_H

/**@}*/
