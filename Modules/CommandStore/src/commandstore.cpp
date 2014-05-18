#include "commandstore.h"
#include <QString>
#include <QStringList>
#include <QVariant>
#include "nglobalcmd.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "commandinterpreter.h"

COMMANDSTORE_BEGIN_NAMESPACE

using namespace ICONSOLE_NAMESPACE;

int ccb_Echo(const CommandSenderInfo &info, const QStringList &args, QVariant &output)
{
    QString message;
    bool first = true;
    
    // For each of the arguments:
    foreach(QString s, args)
    {
        // Add them to our output string, adding a space if necessary.
        message.append(QString((first ? "" : " ") + s));
        first = false;
    }
    
    // Write the string to the console.
    info.writeMessage(message + QString("\n"));
    
    // Set the string as the output.
    output.setValue(message);
    
    // Return OK.
    return NGlobalCmd::CCR_OK;
}

int ccb_Con_TestColours(const CommandSenderInfo &info, const QStringList &args, QVariant &output)
{
    Q_UNUSED(args)
    Q_UNUSED(output)
    
    info.writeOutput(CommandSenderInfo::OutputGeneral, "Normal message.\n");
    info.writeOutput(CommandSenderInfo::OutputWarning, "Warning message.\n");
    info.writeOutput(CommandSenderInfo::OutputCustom1, "Custom colour 1 (inactive text).\n");
    info.writeOutput(CommandSenderInfo::OutputCustom2, "Custom colour 2 (highlighted text).\n");
    info.writeOutput(CommandSenderInfo::OutputCustom3, "Custom colour 3.\n");
    info.writeOutput(CommandSenderInfo::OutputCustom4, "Custom colour 4.\n");
    info.writeOutput(CommandSenderInfo::OutputCustom5, "Custom colour 5.\n");
    info.writeOutput(CommandSenderInfo::OutputCustom6, "Custom colour 6.\n");
    
    return NGlobalCmd::CCR_OK;
}

int ccb_Find(const CommandSenderInfo &info, const QStringList &args, QVariant &output)
{
    Q_UNUSED(output);
    
    // If we don't have the right number of arguments, print help text.
    if (args.count() < 1 || args.at(0).isNull())
    {
        info.writeMessage(QString("Usage: %0 <string>\n").arg(info.commandName()));
        return -1;
    }
    
    // Get the command manager.
    const CommandManager* manager = info.commandManager();
    if ( !manager ) return -1;
    
    QList<BaseConsoleCommand*> list;
    manager->findRegex(QRegularExpression(QString(".*%0.*").arg(QRegularExpression::escape(args.at(0)))), list);
    QStringList namelist;
    
    foreach (BaseConsoleCommand* com, list)
    {
        // Shouldn't happen.
        Q_ASSERT(com);
        if ( !com ) continue;
        
        // Command is variable.
        if ( com->identify() == NGlobalCmd::CIVariable )
        {
            // "Set" the variable with no arguments.
            // This will cause it to print to the console.
            ConVar* cv = dynamic_cast<ConVar*>(com);
            if ( cv )
            {
                info.writeWarning(QString("\"%0\" = %1\n").arg(cv->name()).arg(cv->stringValue()));
                info.writeMessage(QString("- %0\n\n").arg(cv->description()));
                namelist.append(cv->name());
            }
        }
        
        // Command is not variable.
        else
        {
            info.writeWarning(QString("\"%0\"\n").arg(com->name()));
            info.writeMessage(QString("- %0\n\n").arg(com->description()));
            namelist.append(com->name());
        }
    }
    
    return NGlobalCmd::CCR_OK;
}

int ccb_Exec(const CommandSenderInfo &info, const QStringList &args, QVariant &output)
{
    Q_UNUSED(output)
    
    // If we don't have the right number of arguments, print help text.
    if (args.count() < 1 || args.at(0).isNull())
    {
        info.writeMessage(QString("Usage: %0 <filename>\n").arg(info.commandName()));
        return -1;
    }
    
    // Get the command manager.
    CommandManager* manager = info.commandManager();
    if ( !manager ) return -1;
    
    // Attempt to load the file.
    QString fullpath = manager->baseDirectory() + QString("/") + args.at(0);
    QFile file(fullpath);
    QString filename = file.fileName();
    
    //info.writeMessage(QString("Executing %0\n").arg(fullpath));
    
    // Check if file exists.
    if ( !QFile::exists(fullpath) )
    {
        info.writeWarning(QString("Cannot exec %0: file does not exist.\n").arg(filename));
        return -1;
    }
    
    if ( !file.open(QIODevice::ReadOnly|QIODevice::Text) )
    {
        info.writeWarning(QString("Cannot exec %0: unable to open file.\n").arg(filename));
        return -1;
    }
    
    // Create an interpreter.
    CommandInterpreter interpreter(manager);
    
    // Continually read lines from the file.
    QTextStream in(&file);
    while ( !in.atEnd() )
    {
        // Send the line to be interpreted.
        interpreter.parse(in.readLine(), false);
    }
    
    // Close the file.
    file.close();
    
    return NGlobalCmd::CCR_OK;
}

COMMANDSTORE_END_NAMESPACE
