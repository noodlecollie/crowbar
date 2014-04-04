#include "wr_commandinterpreter.h"
#include "regexutil.h"
#include <QByteArray>

ICONSOLE_BEGIN_NAMESPACE

CommandInterpreter::CommandInterpreter(QObject *parent) :
    QObject(parent), m_pCommandManager(NULL)
{
}

CommandInterpreter::CommandInterpreter(CommandManager *manager, QObject *parent) :
    QObject(parent), m_pCommandManager(manager)
{
    connectSignals(m_pCommandManager);
}

void CommandInterpreter::setCommandManager(CommandManager *manager)
{
    bool different = manager != m_pCommandManager;
    // If manager is different, disconnect current signals.
    if ( different )
    {
        disconnectSignals(m_pCommandManager);
    }
    
    m_pCommandManager = manager;
    
    // Whoops, if we don't do this then signals get connected God knows how many times.
    if ( different )
    {
        connectSignals(m_pCommandManager);
    }
}

CommandManager* CommandInterpreter::commandManager() const
{
    return m_pCommandManager;
}

void CommandInterpreter::getSuggestions(const QString &prefix, QList<CommandIdentPair> &list, int count)
{
    //qDebug("Command manager: %p", m_pCommandManager);
    
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
    //QStringList splitFirstPass = cmdString.split(QRegularExpression(NRegexUtil::RegexMatchDelimSemicolon));
    QStringList splitFirstPass;
    splitViaUnquotedChar(splitFirstPass, cmdString, ';');
    
    // Operate on each entry.
    foreach(QString str1, splitFirstPass)
    {
        // Create a list of QPairs.
        CommandEntryPipeList pipeList;
        
        // Split each new string by pipes.
        //QStringList splitSecondPass = str1.split(QRegularExpression(NRegexUtil::RegexMatchDelimPipe));
        QStringList splitSecondPass;
        splitViaUnquotedChar(splitSecondPass, str1, '|');
        
        // Operate on each new entry.
        foreach (QString str2, splitSecondPass)
        {
            // Create a new pair for this command.
            CommandEntryPair pair;
            
            // Retrieve the list of argument matches.
            QRegularExpression matchArgs(REGEXUTIL_NAMESPACE::RegexMatchCommandArgs);
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

void CommandInterpreter::connectSignals(CommandManager* m)
{
    if ( m ) connect(m, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)), this, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)));
}

void CommandInterpreter::disconnectSignals(CommandManager* m)
{
    if ( m ) disconnect(m, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)), this, SIGNAL(outputMessage(CommandSenderInfo::OutputType,QString)));
}

void CommandInterpreter::splitViaUnquotedChar(QStringList &list, const QString &str, char ch, bool shouldTrim)
{
    // Input string's byte array.
    QByteArray inputArray = str.toLatin1();
    
    // Byte array to store temporary split data in.
    QByteArray temp(str.size()+1, '\0');
    int currentPos = 0;
    
    bool inString = false;
    
    for ( int i = 0; i < str.size(); i++ ) // Element at str.size() is '\0'.
    {
        // If character is a backslash, it may be escaping something.
        // If it is escaping a quote, don't change the inString status.
        if ( inputArray[i].operator ==(0x5C) /*Backslash*/ )
        {
            // Next character is a quote.
            if ( inputArray[i+1].operator ==(0x22) /*Quote*/ )
            {
                temp[currentPos] = inputArray[i];
                currentPos++;
                temp[currentPos] = inputArray[i+1];
                currentPos++;
                i++;
                continue;
            }
        }
        
        // If the character is an unescaped quote, toggle our inString status and add to output.
        else if ( inputArray[i].operator ==(0x22) /*Quote*/ )
        {
            inString = !inString;
            temp[currentPos] = inputArray[i];
            currentPos++;
            continue;
        }
        
        // If the character matches our given parameter, don't include it unless it's quoted.
        else if ( inputArray[i].operator ==(ch) )
        {
            // We're in a quoted string, include the character.
            if ( inString )
            {
                temp[currentPos] = inputArray[i];
                currentPos++;
            }
            // We're not in a quoted string - set the character to null and add our current string to the string list.
            else
            {
                temp[currentPos] = '\0';
                currentPos++;
                
                // If the string is longer than one character (which will be null), add to list.
                if ( currentPos > 1 )
                {
                    list.append(shouldTrim? QString(temp.constData()).trimmed() : QString(temp.constData()));
                }
                
                // Reset this for the next iterations.
                currentPos = 0;
            }
        }
        
        // Otherwise, add the character.
        else
        {
            temp[currentPos] = inputArray[i];
            currentPos++;
        }
    }
    
    // We've reached the end of the original string - check the last bit of data we got.
    // If the string is longer than one character (which will be null), add to list.
    if ( currentPos > 1 )
    {
        list.append(shouldTrim? QString(temp.constData()).trimmed() : QString(temp.constData()));
    }
}

ICONSOLE_END_NAMESPACE
