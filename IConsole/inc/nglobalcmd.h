/** \file nglobalcmd.h
 * \brief Defines global properties for console commands.
 */

#ifndef NGLOBALCMD_H
#define NGLOBALCMD_H

#include <QString>
#include <QVariant>

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
    
    typedef int (*CmdCallback) (const QString &name, const QStringList &args, QVariant &output);
    typedef void (*VarCallback) (const QString &oldValue, QString &newValue);
}

#endif // NGLOBALCMD_H
