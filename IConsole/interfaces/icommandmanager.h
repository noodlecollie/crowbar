/** \file icommandmanager.h
 * \brief Defines the command manager interface.
 */

#ifndef ICOMMANDMANAGER_H
#define ICOMMANDMANAGER_H

#include "iconsole_global.h"

class QString;
class QStringList;
class QVariant;

class ICONSOLESHARED_EXPORT ICommandManager
{
    
public:
    virtual ~ICommandManager() {}
    
    // Callback for commands:
    // - name of command
    // - list of arguments
    // - returns command status
    typedef int (*CmdCallback) (const QString&, const QStringList&);
    
    // Variable change callback:
    // - old value of variable
    // - new value, overridable
    typedef void (*VarCallback) (const QString&, QString&);
    
    virtual bool registerCommand(QString &name, CmdCallback callback, QString &description = "", unsigned int flags = 0) = 0;
    virtual bool registerVariable(QString &name, VarCallback callback = NULL, QString &description = "", unsigned int flags = 0,
                                  bool hasMin = false, float min = 0.0, bool hasMax = false, float max = 0.0) = 0;
    virtual void unregister(QString &name) = 0;
    virtual int exec(QString &name, QVariant* const variable = NULL) = 0;
    virtual void suggest(const QString &partialCmd, QStringList &suggestions) = 0;
};

//! \def ICommandManager_iid
//! \brief Unique ID for ICommandManager interface.
#define ICommandManager_iid "Crowbar.Interfaces.IConsole.ICommandManager"
Q_DECLARE_INTERFACE(ICommandManager, ICommandManager_iid)

#endif // ICOMMANDMANAGER_H
