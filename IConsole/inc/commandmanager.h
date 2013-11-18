/** \file commandmanager.h
 * \brief Defines the CommandManager class which allows quick searching for a console command
 * or variable by name.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "iconsole_global.h"
#include <QObject>
#include <QString>
#include <QRegularExpression>
#include "nglobalcmd.h"
#include "commandsenderinfo.h"

class BaseConsoleCommand;
class ConCommand;
class ConVar;

/**
 * @brief Allows quick searching for console commands and variables by their name.
 *
 * The CommandManager indexes commands in a QMap, providing access to a command in logarithmic time. The commands are alphabetically
 * ordered by name.
 */
class ICONSOLESHARED_EXPORT CommandManager : public QObject
{
    Q_OBJECT
public:
    
    /**
     * @brief Typedef for a command entry. The entry contains the name of the command and a BaseConsoleCommand
     * pointer to the command object.
     */
    typedef QMap<QString, BaseConsoleCommand*> BaseCommandMap;
    
    /**
     * @brief Constructor.
     * @param parent QObject parent, if applicable.
     */
    explicit CommandManager(QObject* parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~CommandManager() {}
    
    /**
     * @brief Resigters a command into the manager.
     * @param command Pointer to command to register.
     * @return True on success, false if a command with the same name was already registered.
     */
    bool registerCommand(BaseConsoleCommand *command);
    
    /**
     * @brief Unregisters a command by name.
     * @param name Name of command to remove.
     */
    void unregisterCommand(const QString &name);
    
    /**
     * @brief Returns whether the manager holds a command of the given name.
     * @param name Name of command.
     * @return True if a command with this name exists in the manager, false otherwise.
     */
    bool isRegistered(const QString &name) const;
    
    /**
     * @brief Retrieves a BaseConsoleCommand pointer to the command with the given name.
     * @param name Name of command to retrieve.
     * @return Pointer to command, or NULL if not found.
     */
    BaseConsoleCommand* get(const QString &name) const;
    
    /**
     * @brief Finds commands whose name matches the given regular expression.
     * @param regex Regex to match names to.
     * @param outList List to fill with pointers to matched commands.
     * @param count Maximum number of commands to return. -1 = no maximum.
     */
    void findRegex(const QRegularExpression &regex, QList<BaseConsoleCommand *> &outList, int count = -1) const;
    
    /**
     * @brief Finds commands whose name matches the given regular expression.
     * @param regex Regex string to match names to.
     * @param outList List to fill with pointers to matched commands.
     * @param count Maximum number of commands to return. -1 = no maximum.
     */
    void findRegex(const QString &regex, QList<BaseConsoleCommand *> &outList, int count = -1) const;
    
    /**
     * @brief Finds commands whose name matches the given prefix - for returning command suggestions for a
     * partially typed command name.
     * @param prefix Prefix to search with.
     * @param outList List to fill with pointers to matched commands.
     * @param count Maximum number of commands to return. -1 = no maximum.
     */
    void findPrefix(const QString &prefix, QList<BaseConsoleCommand *> &outList, int count = -1) const;
    
    /**
     * @brief Returns a console command with the given name.
     * @param name Name of console command to search for.
     * @return Pointer to command, or NULL if command didn't exist or did not identify as NGlobalCmd::CICommand.
     */
    ConCommand* getCommand(const QString &name) const;
    
    /**
     * @brief Returns a console variable with the given name.
     * @param name Name of console variable to search for.
     * @return Pointer to variable, or NULL if variable didn't exist or did not identify as NGlobalCmd::CIVariable.
     */
    ConVar* getVariable(const QString &name) const;
    
    /**
     * @brief Executes a command of the given name, with the given arguments, and fills retVal and output with
     * the relevant output data.
     *
     * <ul><li>If the command is a ConCommand, \a retVal holds the return value of the command and the return of this
     * function is NGlobalCmd::CICommand.</li>
     * <li>If the command is a ConVar, the first argument in \a args (if it exists) is taken as the value for the variable
     * to be set to. \a Output holds the value of the variable (which will be the new value if the variable was set
     * via the arguments). If no argument is specified, the value of and details about the variable are printed to the console window.
     * The return value of the function is NGlobalCmd::CIVariable.</li>
     * <li>If the command is not found, or is neither a ConCommand nor a ConVar, \a retVal and \a output are unchanged and
     * the function returns NGlobalCmd::CINull.</li></ul>
     * @param name Name of command to search for.
     * @param args Arguments to provide to the command.
     * @param retVal Holds the return value if the command is a ConCommand.
     * @param output Holds the evaluated value of the variable if the command is a ConVar.
     * @return Identifier depending on the type of command that was found.
     */
    NGlobalCmd::CmdIdent exec(const QString &name, const QStringList &args, int &retVal, QVariant &output);
    
    /**
     * @brief Returns the const_iterator representing the beginning of the QMap.
     * @return Const_iterator representing the beginning of the QMap.
     */
    BaseCommandMap::const_iterator constBegin();
    
    /**
     * @brief Returns the const_iterator representing the null item after the end of the QMap.
     * @return Const_iterator representing the null item after the end of the QMap.
     */
    BaseCommandMap::const_iterator constEnd();
    
signals:
    /**
     * @brief Emitted to signal to the command interpreter than output is to be written to the console window.
     */
    void outputMessage(CommandSenderInfo::OutputType, const QString&);
    
public slots:
    
private:
    /**
     * @brief Causes the outputMessage signal to be emitted.
     * @param type Type of output to specify.
     * @param message Message to print as output.
     */
    inline void emitMessage(CommandSenderInfo::OutputType type, const QString &message);
    
    // Convenience functions.
    
    /**
     * @brief Convenience function - emits a message.
     * @param message Message text.
     */
    inline void message(const QString &message) { emit outputMessage(CommandSenderInfo::OutputGeneral, message); }
    
    /**
     * @brief Convenience function - emits a warning.
     * @param warning Warning text.
     */
    inline void warning(const QString &warning) { emit outputMessage(CommandSenderInfo::OutputWarning, warning); }
    
    BaseCommandMap     m_CommandMap;    /**< QMap to hold commands. */
};

#endif // COMMANDMANAGER_H

/**@}*/
