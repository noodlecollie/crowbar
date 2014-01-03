/** \file listedcommandmanager.h
 * \brief Defines the listed command manager class.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef LISTEDCOMMANDMANAGER_H
#define LISTEDCOMMANDMANAGER_H

#include "iconsole_global.h"
#include "commandmanager.h"

ICONSOLE_BEGIN_NAMESPACE

class ListedConsoleCommand;
class InitSignalWrapper;

/**
 * @brief Extends the CommandManager class by providing functionality to traverse a list of ListedConsoleCommands when the
 * manager is created.
 */
class ICONSOLESHARED_EXPORT ListedCommandManager : public CommandManager
{
public:
    /**
     * @brief Constructor.
     * @param parent QObject parent, if applicable.
     */
    explicit ListedCommandManager(QObject* parent = 0);
    
    /**
     * @brief Copy constructor
     * @param other Manager to copy from.
     * @param parent QObject parent, if applicable.
     */
    explicit ListedCommandManager(ListedCommandManager &other, QObject *parent = 0);
    
    /**
     * @brief Constructor with a pointer to a list to traverse.
     * @param listHead When constructed, the manager will traverse the list whose head is pointed to by this parameter.
     * @param parent QObject parent, if applicable.
     */
    explicit ListedCommandManager(ListedConsoleCommand* listHead, QObject* parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~ListedCommandManager() {}
    
    /**
     * @brief Traverse the list specified by the head and register each console command to the manager.
     * @param listHead Head element of the list of ListedConsoleCommand objects.
     */
    void traverse(ListedConsoleCommand* listHead);
};
    
ICONSOLE_END_NAMESPACE

#endif // LISTEDCOMMANDMANAGER_H

/**@}*/
