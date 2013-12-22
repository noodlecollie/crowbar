/** \file listedconsolecommand.h
 * \brief Defines the listed console command class. 
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef LISTEDCONSOLECOMMAND_H
#define LISTEDCONSOLECOMMAND_H

#include "iconsole_global.h"
#include <QString>
#include "nglobalcmd.h"
#include "baseconsolecommand.h"

class CommandManager;

/**
 * @brief Provides functionality for console commands to form a linked list.
 *
 * A ListedCommandManager can then traverse the list of commands
 * and register each one of the individually. This allows for ListedConsoleCommands to be created statically at global scope without having
 * to worry about whether the command manager they wish to register with exists yet; if the manager exists, the command registers immediately.
 * If it does not exist, the command attaches itself to the head of the provided list for the manager to traverse later.
 */
class ICONSOLESHARED_EXPORT ListedConsoleCommand : public BaseConsoleCommand
{
public:
    /**
     * @brief Constructor specifying command manager and list head.
     * @param name Name of the command.
     * @param manager Manager we wish to register to.
     * @param listHead List to attach to if the manager does not exist.
     * @param desc Optional command description.
     * @param flags Command flags.
     * @param parent QObject parent, if applicable.
     */
    explicit ListedConsoleCommand(const QString &name, CommandManager* manager, ListedConsoleCommand** listHead,
                                  const QString &desc = "", NGlobalCmd::ArgCallback argCallback = NULL,
                                  NGlobalCmd::CMDFLAGS flags = 0, QObject *parent = 0);
    
    /**
     * @brief Constructor.
     * @param name Name of the command.
     * @param desc Optional command description.
     * @param flags Command flags.
     * @param parent QObject parent, if applicable.
     */
    explicit ListedConsoleCommand(const QString &name, const QString &desc = "", NGlobalCmd::ArgCallback argCallback = NULL,
                                  NGlobalCmd::CMDFLAGS flags = 0, QObject *parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~ListedConsoleCommand() {}
    
    /**
     * @brief Gets the next command in the linked list.
     * @return Next command in the list, or NULL if this is the last command in the list.
     */
    ListedConsoleCommand* getListNext() const;
    
    /**
     * @brief Sets the next command in the list.
     * @param next Pointer to the next command in the list.
     */
    void setListNext(ListedConsoleCommand *next);
    
    /**
     * @brief Attempts to register with a manager.
     * @param manager Manager to try and register to.
     * @param listHead List to attach to if the manager does not yet exist.
     */
    void tryRegister(CommandManager *manager, ListedConsoleCommand **listHead);
    
signals:
    
public slots:
    
private:
    ListedConsoleCommand*     m_pListNext;  /**< Pointer to the next command in the linked list. */
};

#endif // LISTEDCONSOLECOMMAND_H

/**@}*/
