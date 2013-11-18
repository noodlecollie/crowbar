/** \file concommand.h
 * \brief Defines the ConCommand class, for executing code from the console.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef CONCOMMAND_H
#define CONCOMMAND_H

#include "iconsole_global.h"
#include <QString>
#include <QList>
#include <QVariant>
#include "nglobalcmd.h"
#include "listedconsolecommand.h"

class CommandSenderInfo;

class CommandManager;

/**
 * @brief Defines code which is executed whenever the user enters the command name into the console.
 *
 * ConCommands work almost exactly the same way as in the Source engine: they contain a pointer to a method which
 * is executed when the command is typed by the user. When a ConCommand is executed, it returns an integer status
 * (usually corresponding to an NGlobalCmd::ConCommandReturn value) and can optionally return arbitrary output in
 * the output QVariant. ConCommands can write output to the a console window (relevant to the calling CommandManager,
 * if one exists) through the passed CommandSenderInfo.
 */
class ICONSOLESHARED_EXPORT ConCommand : public ListedConsoleCommand
{
public:
    /**
     * @brief Constructor.
     * @param name Name of the command.
     * @param callback Pointer to callback function to run when this command is executed.
     * @param desc Optional command description.
     * @param flags Command flags.
     * @param parent QObject parent, if applicable.
     */
    explicit ConCommand(const QString &name, NGlobalCmd::CmdCallback callback, const QString &desc = "",
                            NGlobalCmd::CMDFLAGS flags = 0, QObject *parent = 0);
    
    /**
     * @brief Constructor passing in manager and list to attempt to register to.
     * @param name Name of the command.
     * @param callback Pointer to callback function to run when this command is executed.
     * @param manager Manager to attempt to register to when this command is constructed.
     * @param list If the command could not register with the manager, it attaches itself to this list instead.
     * @param desc Optional command description.
     * @param flags Command flags.
     * @param parent QObject parent, if applicable.
     */
    explicit ConCommand(const QString &name, NGlobalCmd::CmdCallback callback, CommandManager* manager,
                            ListedConsoleCommand** list, const QString &desc = "", const NGlobalCmd::CMDFLAGS flags = 0, QObject* parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~ConCommand() {}
    
    /**
     * @brief Returns the identifier for this command.
     * @return CmdIdent::CICommand for console commands.
     */
    virtual NGlobalCmd::CmdIdent identify() const;
    
    /**
     * @brief Runs the console command's execution function.
     * @param info Info class to facilitate communication between the command and the caller.
     * @param args Arguments to the command.
     * @param output QVariant to hold any output from the command.
     * @return Integer return code of the command function.
     */
    int exec(const CommandSenderInfo &info, const QStringList &args, QVariant &output);
    
    /**
     * @brief Gets the callback function pointer for this console command.
     * @return Pointer to callback function to run when this command is executed.
     */
    NGlobalCmd::CmdCallback getExec() const;
    
    /**
     * @brief Sets the callback function that is run when this command is executed.
     * @param cmd Pointer to callback.
     */
    void setExec(NGlobalCmd::CmdCallback cmd);
    
signals:
    
public slots:
    
private:
    NGlobalCmd::CmdCallback    m_pCallback; /**< Pointer to execution callback */
};

#endif // CONCOMMAND_H

/**@}*/
