/** \file iconsolewindow.h
 * \brief Interface for console window.
 */

#ifndef ICONSOLEWINDOW_H
#define ICONSOLEWINDOW_H

#include "iconsole_global.h"

class ICONSOLESHARED_EXPORT IConsoleWindow
{
public:
    virtual ~IConsoleWindow() {}
    
    virtual void printMessage(const QString &message) = 0;
    virtual void printWarning(const QString &message) = 0;
};

//! \def IConsoleWindow_iid
//! \brief Unique ID for IConsoleWindow interface.
#define IConsoleWindow_iid "Crowbar.Interfaces.IConsole.IConsoleWindow"
Q_DECLARE_INTERFACE(IConsoleWindow, IConsoleWindow_iid)

#endif // ICONSOLEWINDOW_H
