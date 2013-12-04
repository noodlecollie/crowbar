#include "wr_commandinterpreter.h"
#include "regexutil.h"

//const QRegularExpression CommandInterpreter::matchArgs = QRegularExpression("\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\"|[\\S]+");

//// Strict matches spaces between arguments as well - used to tell whether we have finished typing one argument.
//const QRegularExpression CommandInterpreter::matchArgsStrict = QRegularExpression("\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\"|[\\S]+|[\\s]+");

//const QRegularExpression CommandInterpreter::delimPipes = QRegularExpression("\\s*(?<!\\\\)\\|\\s*");
//const QRegularExpression CommandInterpreter::delimSemicolons = QRegularExpression("\\s*(?<!\\\\)\\;\\s*");
//const QRegularExpression CommandInterpreter::matchLastArgumentNonWhitespace = QRegularExpression("\\S*$");
//const QRegularExpression CommandInterpreter::matchLastArgumentWordChar = QRegularExpression("\\w*$");

CommandInterpreter::CommandInterpreter(QObject *parent) :
    QObject(parent), m_pCommandManager(NULL)
{
}

CommandInterpreter::CommandInterpreter(CommandManager *manager, QObject *parent) :
    QObject(parent), m_pCommandManager(manager)
{
    connectSignals();
}

void CommandInterpreter::setCommandManager(CommandManager *manager)
{
    m_pCommandManager = manager;
    connectSignals();
}

CommandManager* CommandInterpreter::commandManager() const
{
    return m_pCommandManager;
}

void CommandInterpreter::getSuggestions(const QString &prefix, QList<CommandIdentPair> &list, int count)
{
    // If manager is NULL, return.
    if ( !m_pCommandManager ) return;
    
    // Call findPrefix on the manager.
    QList<BaseConsoleCommand*> cmdList;
    m_pCommandManager->findPrefix(prefix, cmdList, count);
    
    // For each of the returned commands, create an entry in our output list.
    foreach (BaseConsoleCommand* cmd, cmdList)
    {
        list.append(QPair<NGlobalCmd::CmdIdent, QString>(cmd->identify(), cmd->name()));
    }
}

void CommandInterpreter::parse(const QString &cmdString)
{
    // Steps:
    // - Create a list to hold <QString, QStringList> pairs. The QString is the command name, the list is its arguments.
    // - If pipes are present, the list will be longer than a single element. If a command is to receive piped output from
    //      a previous command, a $ symbol should signify the point in the arguments list to insert the received output.
    //      The $ symbol should be on its own as a single argument.
    //      If no $ argument is found, the arguments are appended to the end of the existing argument list.
    //      If multiple $ argument are found, only the first is used.
    //      Output is inserted into the argument list as a zero or more strings. These strings are inserted exactly as they are presented
    //      from the previous command - if the output is a string list, each list entry will become a new argument; if the output is one
    //      long string, the string will become one argument without being delimited at any characters. Arguments output as numbers
    //      (int/float/etc.) are cast to a string before being passed.
    // - Once the list has been compiled, each command should be executed sequentially.
    
    // If we have no manager, don't bother running this.
    if ( !m_pCommandManager ) return;
    
    QString newCmdString = cmdString.trimmed();
    
    // Echo back our command string.
    m_pCommandManager->outputMessage(CommandSenderInfo::OutputGeneral, QString("] %0\n").arg(newCmdString));
    
    // Create a command entry list.
    CommandEntryList masterList;
    
    // Parse our command string.
    parseCommandString(newCmdString, masterList);
    
    // Iterate through the pipe lists:
    foreach(CommandEntryPipeList pipeList, masterList)
    {
        // String list of arguments we need to pipe to the next command in the pipe list.
        QStringList pipeArgs;
        
        // For each of the commands:
        foreach(CommandEntryPair pair, pipeList)
        {
            int returnVal = NGlobalCmd::CCR_OK;
            QVariant output;
            
            // Insert stored output if we need to.
            if ( pipeArgs.count() > 0 )
            {
                // Find the index of the first single argument comprised of "$".
                // We allow for whitespace here.
                int index = pair.second.indexOf(QRegularExpression("^\\s*\\$\\s*$"));
                
                // If the index existed:
                if ( index >= 0 )
                {
                    // Remove the index of "$" first.
                    pair.second.removeAt(index);
                    
                    foreach(QString str, pipeArgs)
                    {
                        // Insert at the index.
                        pair.second.insert(index, str);
                        
                        // Increment the index for the next insertion.
                        index++;
                    }
                }
                // Otherwise we need to append to the list instead.
                else
                {
                    foreach(QString str, pipeArgs)
                    {
                        // Append.
                        pair.second.append(str);
                    }
                }
            }
            
            // If the command was found successfully:
            if ( m_pCommandManager->exec(pair.first, pair.second, returnVal, output) != NGlobalCmd::CINull )
            {
                // Convert the output to a string list.
                pipeArgs = output.toStringList();
                
                // Not sure at this stage whether toStringList will work with values that aren't strings,
                // so I'm going to workaround if the value didn't copy over.
                // Might be a bit hackish, keep an eye on this.
                if ( pipeArgs.count() < 1 )
                {
                    // If toString doesn't work, toStringList wouldn't have worked anyway.
                    pipeArgs = QStringList(output.toString());
                }
            }
            // Otherwise, blank the output.
            else
            {
                pipeArgs.clear();
            }
        }
    }
}

// Given a command string, filled the passed list with the commands and arguments contained in the string.
void CommandInterpreter::parseCommandString(const QString &cmdString, CommandEntryList &masterList)
{
    // Split the original string by semicolons.
    QStringList splitFirstPass = cmdString.split(QRegularExpression(NRegexUtil::RegexMatchDelimSemicolon));
    
    // Operate on each entry.
    foreach(QString str1, splitFirstPass)
    {
        // Create a list of QPairs.
        CommandEntryPipeList pipeList;
        
        // Split each new string by pipes.
        QStringList splitSecondPass = str1.split(QRegularExpression(NRegexUtil::RegexMatchDelimPipe));
        
        // Operate on each new entry.
        foreach (QString str2, splitSecondPass)
        {
            // Create a new pair for this command.
            CommandEntryPair pair;
            
            // Retrieve the list of argument matches.
            QRegularExpression matchArgs(NRegexUtil::RegexMatchCommandArgs);
            QRegularExpressionMatchIterator m = matchArgs.globalMatch(str2);
            
            while ( m.hasNext() )
            {
                QString toAdd = m.next().captured(0);
                
                // If there are quotes at the start/end of the arg, remove them.
                // We -shouldn't- need to worry about trailing whitespace because the regular expression should strip this out.
                if ( toAdd.startsWith('"', Qt::CaseInsensitive) && toAdd.endsWith('"', Qt::CaseInsensitive) && toAdd.length() > 1 )
                {
                    toAdd.remove(toAdd.length() - 1, 1);
                    toAdd.remove(0, 1);
                }
                
                // Replace any escaped quotes with quotes.
                // Original regex string is \\" but each of these three characters needs escaping!
                toAdd.replace(QRegularExpression("\\\\\""), "\"");
                
                pair.second.append(toAdd);
            }
            
            // If the number of arguments found is < 1, insert a blank command into the list.
            if ( pair.second.count() < 1 )
            {
                pair.first = QString("");
            }
            // Otherwise, set the command name to be the first in the argument list.
            else
            {
                pair.first = pair.second.takeAt(0);
            }
            
            // Add the pair to the pipe list.
            pipeList.append(pair);
        }
        
        // Add the list of pairs to the main list.
        masterList.append(pipeList);
    }
}

void CommandInterpreter::connectSignals()
{
    if ( m_pCommandManager ) connect(m_pCommandManager, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)), this, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)));
}
