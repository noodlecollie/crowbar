/** \file nglobalcmd.h
 * \brief Defines global properties for console commands.
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
    enum CmdIdent
    {
        CINull = 0,
        CICommand,
        CIVariable
    };
    
    enum ConCommandReturn
    {
        CCR_OK = 0,
        CCR_NoCallback
    };
    
    // Command flags
    typedef unsigned int CMDFLAGS;
    
    // Change this to a Q_ENUM!
    enum
    {
        CMDFLAG_READONLY = 0x1, // ConVar is not allowed to be changed from the default, only read.
        CMDFLAG_ENSURECALLBACK  // Command or variable must have a callback.
    };  
    
    typedef int (*CmdCallback) (const CommandSenderInfo &info, const QStringList &args, QVariant &output);
    typedef void (*VarCallback) (const CommandSenderInfo &info, const QString &oldValue, QString &newValue);
}

#endif // NGLOBALCMD_H
