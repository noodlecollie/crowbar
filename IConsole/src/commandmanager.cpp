#include "wr_commandmanager.h"

CommandManager::CommandManager(QObject *parent) : QObject(parent), m_CommandMap()
{
}

bool CommandManager::registerCommand(BaseConsoleCommand *command)
{
    // Don't register if there is already a command with this name in the manager.
    if ( get(command->getName()) )
    {
        return false;
    }

    // Add the command reference to the map.
    m_CommandMap.insert(command->getName(), command);
    return true;
}

void CommandManager::unregisterCommand(const QString &command)
{
    m_CommandMap.remove(command);
}

bool CommandManager::isRegistered(const QString &command) const
{
    return m_CommandMap.contains(command);
}

BaseConsoleCommand* CommandManager::get(const QString &command) const
{
    // Try and find the command.
    BaseCommandMap::const_iterator it = m_CommandMap.find(command);

    // If we didn't find it, return NULL.
    if ( it == m_CommandMap.end() ) return NULL;

    // Return the address of the object.
    return it.value();
}

void CommandManager::findRegex(const QRegularExpression &regex, QList<BaseConsoleCommand*> &commands) const
{
    // Unfortunately we'll need to do a linear search here.
    foreach(BaseConsoleCommand* cmd, m_CommandMap)
    {
        // If the name matches the regex, add the command to the output list.
        if ( regex.match(cmd->getName()).hasMatch() ) commands.append(cmd);
    }
}

void CommandManager::findPrefix(const QString &prefix, QList<BaseConsoleCommand*> &commands) const
{
    // Wrapper for the RegEx matcher, but checks for prefixes only.
    // We'll need to use our prefix string to generate a regex to pass.

    // <start>[prefix](any char any number of times...)<end>
    QString regStr = "^" + QRegularExpression::escape(prefix) + ".*$";
    QRegularExpression regex(regStr);
    findRegex(regex, commands);
}

ConCommand* CommandManager::getCommand(const QString &name) const
{
    BaseConsoleCommand* cmd = get(name);

    // If we are not a ConCommand, return NULL.
    if ( cmd->identify() != NGlobalCmd::CICommand ) return NULL;
    
    // Return a cast to ConCommand.
    return (ConCommand*) cmd;
}

ConVar* CommandManager::getVariable(const QString &name) const
{
    BaseConsoleCommand* cmd = get(name);

    // If we are not a ConVar, return NULL.
    if ( cmd->identify() != NGlobalCmd::CIVariable ) return NULL;
    
    // Return a cast to ConVar.
    return (ConVar*) cmd;
}

NGlobalCmd::CmdIdent CommandManager::exec(const QString &name, const QStringList &args, int &retVal, QVariant &output)
{
    // Find the command in the manager.
    BaseConsoleCommand* cmd = get(name);

    // If the command does not exist, return null.
    if ( !cmd ) return NGlobalCmd::CINull;

    // Deal with ident.
    switch ( cmd->identify() )
    {
        case NGlobalCmd::CICommand:
        {
            ConCommand* con = (ConCommand*) cmd;
            
            // Execute the command.
            retVal = con->exec(name, args, output);
    
            return NGlobalCmd::CICommand;
        }
        
        case NGlobalCmd::CIVariable:
        {
            ConVar* var = (ConVar*) cmd;
            
            // Get or set the variable.
            if ( args.count() < 1 ) output.setValue(var->get());
            else output.setValue(var->set(args.at(0)));
            return NGlobalCmd::CIVariable;
        }
        
        default:
        {
            return NGlobalCmd::CINull;
        }
    }
}
