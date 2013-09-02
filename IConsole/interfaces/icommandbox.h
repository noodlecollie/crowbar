/** \file icommandbox.h
 * \brief Interface for command box (where commands are typed).
 */

#ifndef ICOMMANDBOX_H
#define ICOMMANDBOX_H

#include "iconsole_global.h"

class ICONSOLESHARED_EXPORT ICommandBox
{
public:
    virtual ~ICommandBox() {}
    
    virtual void showSuggestions(const QStringList &list) = 0;
};

//! \def ICommandBox_iid
//! \brief Unique ID for ICommandBox interface.
#define ICommandBox_iid "Crowbar.Interfaces.IConsole.ICommandBox"
Q_DECLARE_INTERFACE(ICommandBox, ICommandBox_iid)

#endif // ICOMMANDBOX_H
