/** \file commandinterpreter.h
 * \brief Defines the CommandInterpreter class to interpret raw user-input command strings.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef COMMANDINTERPRETER_H
#define COMMANDINTERPRETER_H

#include "iconsole_global.h"
#include "nglobalcmd.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QPair>
#include <QRegularExpression>

ICONSOLE_BEGIN_NAMESPACE

class CommandManager;

/**
 * @brief Handles parsing strings of user input and executing the relevant commands.
 *
 * When a user enters commands into a CommandEntryBox, the resulting command string is sent to a CommandInterpreter.
 * The interpreter handles tasks such as searching for the commands the user has entered, executing them and piping
 * output from one command into another.
 *
 * After parsing a command string, the format of the commands is as follows:
 *
 * <code>LIST <- Main list. Sub-lists are delimited by semicolons in the user input string.<br/>
 * {<br/>
 * &nbsp;&nbsp;List    <- Sub-list. Sequential commands pipe output to input. Commands in this list are delimited by pipes in the user input string.<br/>
 * &nbsp;&nbsp;{<br/>
 * &nbsp;&nbsp;&nbsp;&nbsp;Pair(string, StringList)    <- Pair of command string and list of arguments. Executed first.<br/>
 * &nbsp;&nbsp;&nbsp;&nbsp;Pair(string, StringList)    <- Executed second. Receives input of first.<br/>
 * &nbsp;&nbsp;&nbsp;&nbsp;...<br/>
 * &nbsp;&nbsp;}<br/>
 *<br/>
 * &nbsp;&nbsp;List<br/>
 * &nbsp;&nbsp;{<br/>
 * &nbsp;&nbsp;&nbsp;&nbsp;Pair(String, StringList)    <- Executed next. Does not receive any input.<br/>
 * &nbsp;&nbsp;&nbsp;&nbsp;Pair(string, StringList)    <- Executed next again, receiving above command's input.<br/>
 * &nbsp;&nbsp;&nbsp;&nbsp;...<br/>
 * &nbsp;&nbsp;}<br/>
 *<br/>
 * &nbsp;&nbsp;...<br/>
 * }</code><br/>
 *
 * An example:
 * <b>command1 arg1 | command2 arg1 $ arg3 ; command3</b>
 *
 * Command1 is executed first. Command2 receives the output of command1 at the point where the dollar symbol lies,
 * and command3 receives nothing as input.
 *
 * If the argument list for command2 contained more than one dollar symbol, the output from the previous command would
 * only be inserted at the position of the first symbol.
 *
 * If the argument list for command2 contained no dollar symbol, the output from command1 would be appended to the end of
 * command2's input list.
 */
class ICONSOLESHARED_EXPORT CommandInterpreter : public QObject
{
    Q_OBJECT
    /** @property commandManager
     * @brief Command manager the interpreter is linked to.
     * @accessors commandManager(), setCommandManager()
     */
    Q_PROPERTY(CommandManager* commandManager READ commandManager WRITE setCommandManager)
public:
    /**
     * @brief Typedef for a pair containing a string and string list. The string corresponds to a command name
     * and the string list to its arguments.
     */
    typedef QPair<QString, QStringList> CommandEntryPair;
    
    /**
     * @brief Typedef for a list of command entry pairs. Each command's output is piped into the input of the next command.
     */
    typedef QList<CommandEntryPair> CommandEntryPipeList;
    
    /**
     * @brief Typedef for a list of command entry pipe lists. Each pipe list is executed sequentially.
     */
    typedef QList<CommandEntryPipeList> CommandEntryList;
    
    /**
     * @brief Typedef for a pair containing a command identifier and its string name.
     */
    typedef QPair<NGlobalCmd::CmdIdent, QString> CommandIdentPair;
    
    /**
     * @brief Constructor passing in command manager.
     * @param manager Manager to link this interpreter to.
     * @param parent QObject parent, if applicable.
     */
    explicit CommandInterpreter(CommandManager* manager, QObject *parent = 0);
    
    /**
     * @brief Default constructor.
     * @param parent QObject parent, if applicable.
     */
    explicit CommandInterpreter(QObject *parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~CommandInterpreter() {}
    
    /**
     * @brief Sets the command manager this interpreter is linked to.
     * @param manager Manager to link to.
     */
    void setCommandManager(CommandManager* manager);
    
    /**
     * @brief Gets the manager this interpreter is linked to.
     * @return Manager the interpreter is linked to, or NULL if it is not linked to any manager.
     */
    CommandManager* commandManager() const;
    
    /**
     * @brief Splits the given string into contiguous sections using the given character as a delimiter.
     * If the character is found inside a quoted string section it is ignored.
     * @param list QStringList to output split sections to.
     * @param str String to parse.
     * @param ch Character to split at.
     * @param shouldTrim If true, each split string will have preceding and trailing whitespace trimmed.
     */
    static void splitViaUnquotedChar(QStringList &list, const QString &str, char ch, bool shouldTrim = true);
    
    // Suggestions are returned as a pair containing the ident and the command name.
    //void getSuggestions(const QString &prefix, QList<CommandIdentPair> &list, int count = -1);
    
signals:
    /**
     * @brief Emitted when a command or variable contacted by this interpreter wants to output a message.
     */
    void outputMessage(CommandSenderInfo::OutputType, const QString&);
    
public slots:
    // Parses command string - separates command name and arguments, and determines command pipes.
    // When piping, the output variant is converted to a QStringList to be used as the arguments for the next command.
    
    /**
     * @brief Parses a command string and executes any commands specified by the string.
     * @param cmdString String to parse.
     */
    void parse(const QString &cmdString);
    
    /**
     * @brief Gets suggested commands for the given command prefix.
     * @param prefix The string to be matched against the beginning of pre-existing commands.
     * @param list List to fill with suggestion entries.
     * @param count Maximum number of suggestions desired, or -1 if no limit.
     */
    void getSuggestions(const QString &prefix, QList<CommandInterpreter::CommandIdentPair> &list, int count);
    
private:
    /**
     * @brief Given a command string, fills the passed list with the commands and arguments contained in the string.
     * @param cmdString String to parse.
     * @param masterList List to fill.
     */
    static void parseCommandString(const QString &cmdString, CommandEntryList &masterList);
    
    /**
     * @brief Convenience function - connects required signals/slots.
     */
    void connectSignals(CommandManager* m);
    
    /**
     * @brief Convenience function - disconnects required signals/slots.
     */
    void disconnectSignals(CommandManager* m);
    
    CommandManager*     m_pCommandManager;  /**< Command manager this interpreter is linked to. */
};

ICONSOLE_END_NAMESPACE
    
#endif // COMMANDINTERPRETER_H

/**@}*/
