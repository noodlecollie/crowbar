#include "wr_commandmanager.h"

CommandManager::CommandManager(QObject *parent) : QObject(parent), m_CommandMap()
{
}

bool CommandManager::registerCommand(BaseConsoleCommand *command)
{
    // Don't register if there is already a command with this name in the manager.
    if ( get(command->name()) )
    {
        return false;
    }

    // Add the command reference to the map.
    m_CommandMap.insert(command->name(), command);
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

void CommandManager::findRegex(const QRegularExpression &regex, QList<BaseConsoleCommand*> &commands, int count) const
{
    // Set the maximum number of entries we should append.
    int maxEntries = m_CommandMap.count();
    if ( count >= 0 && count < m_CommandMap.count() ) maxEntries = count;
    
    int numAdded = 0;
    foreach(BaseConsoleCommand* cmd, m_CommandMap)
    {
        // If we've already added as many commands as we're allowed to, break here.
        if ( numAdded >= maxEntries ) break;
        
        // If the name matches the regex, add the command to the output list.
        if ( regex.match(cmd->name()).hasMatch() )
        {
            commands.append(cmd);
            numAdded++;
        }
    }
}

void CommandManager::findRegex(const QString &regex, QList<BaseConsoleCommand *> &outList, int count) const
{
    findRegex(QRegularExpression(regex), outList, count);
}

void CommandManager::findPrefix(const QString &prefix, QList<BaseConsoleCommand*> &commands, int count) const
{
    // Wrapper for the RegEx matcher, but checks for prefixes only.
    // We'll need to use our prefix string to generate a regex to pass.

    // <start>[prefix](any char any number of times...)<end>
    QString regStr = "^" + QRegularExpression::escape(prefix) + ".*$";
    findRegex(regStr, commands, count);
}

ConCommand* CommandManager::getCommand(const QString &name) const
{
    BaseConsoleCommand* cmd = get(name);

    // If we are not a ConCommand, return NULL.
    if ( !cmd || cmd->identify() != NGlobalCmd::CICommand )
    {
        return NULL;
    }
    
    // Return a cast to ConCommand.
    return (ConCommand*) cmd;
}

ConVar* CommandManager::getVariable(const QString &name) const
{
    BaseConsoleCommand* cmd = get(name);

    // If we are not a ConVar, return NULL.
    if ( !cmd || cmd->identify() != NGlobalCmd::CIVariable ) return NULL;
    
    // Return a cast to ConVar.
    return (ConVar*) cmd;
}

NGlobalCmd::CmdIdent CommandManager::exec(const QString &name, const QStringList &args, int &retVal, QVariant &output)
{
    // Handled in interpreter instead!
//    // Print the command we're going to execute.
//    QString argList;
//    foreach(QString s, args)
//    {
//        argList.append(QString(" %0").arg(s));
//    }
//    argList = argList.trimmed();
    
//    message(QString("] %0 %1\n").arg(name).arg(argList));
    
    // If we have no command, return here.
    if ( name.trimmed().isEmpty() )
    {
        return NGlobalCmd::CINull;
    }
    
    // Find the command in the manager.
    BaseConsoleCommand* cmd = get(name);

    // If the command does not exist, return null.
    if ( !cmd )
    {
        warning(QString("%0: Command not found.\n").arg(name));
        return NGlobalCmd::CINull;
    }
    
    // Crete sender info.
    CommandSenderInfo info(cmd->name(), this, &CommandManager::emitMessage);
    
    // Process arguments to remove quotes.
    // Unfortunately this means building a new argument list.
//    QStringList newArgs;
//    foreach(QString s, args)
//    {
//        // Replace any unescaped quotes with nothing.
//        s.replace(QRegularExpression("(?<!\\\\)\"(?=[\\S]+)"), "");
        
//        // Replace any escaped quotes with unescaped quotes.
//        s.replace(QRegularExpression("\\\\(?=\\\"[\\S]+)"), "");
//        newArgs.append(s);
//    }

    // Deal with ident.
    switch ( cmd->identify() )
    {
        case NGlobalCmd::CICommand:
        {
            ConCommand* con = (ConCommand*) cmd;
            
            // Execute the command.
            retVal = con->exec(info, args, output);
    
            return NGlobalCmd::CICommand;
        }
        
        case NGlobalCmd::CIVariable:
        {
            ConVar* var = (ConVar*) cmd;
            
            // Get or set the variable.
            if ( args.count() < 1 )     // Get
            {
                output.setValue(var->get());
                
                // Send an output signal with the value.
                warning(QString("\"%0\" = \"%1\" (def. \"%2\")\n").arg(var->name()).arg(output.toString()).arg(var->getDefault()));
                message(QString("- %0\n").arg(var->description()));
            }
            else                        // Set
            {
                output.setValue(var->set(info, args.at(0)));
            }
            
            return NGlobalCmd::CIVariable;
        }
        
        default:
        {
            return NGlobalCmd::CINull;
        }
    }
}

CommandManager::BaseCommandMap::const_iterator CommandManager::constBegin()
{
    return m_CommandMap.constBegin();
}

CommandManager::BaseCommandMap::const_iterator CommandManager::constEnd()
{
    return m_CommandMap.constEnd();
}

void CommandManager::emitMessage(CommandSenderInfo::OutputType type, const QString &message)
{
    emit outputMessage(type, message);
}
