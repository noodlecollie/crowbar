#include "commandstore.h"
#include <QString>
#include <QStringList>
#include <QVariant>
#include "nglobalcmd.h"

namespace NCommandStore
{
    using namespace NIConsole;
    
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

    int ccb_Con_TestColours(const CommandSenderInfo &info, const QStringList &/*args*/, QVariant &/*output*/)
    {
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
}
