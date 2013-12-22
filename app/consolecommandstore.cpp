#include "consolecommandstore.h"

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
