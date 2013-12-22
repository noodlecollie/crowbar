#ifndef COMMANDSTORE_H
#define COMMANDSTORE_H

#include "commandstore_global.h"
#include "commandsenderinfo.h"

namespace CommandStore
{
    /**
     * @brief Echoes arguments back to the caller's console and to the output.
     * 
     * Each individual argument is separated by a space.
     * Quoted strings are treated as one argument.
     * To print quotes, escape them with a backslash.
     * @param info Information about the calling command manager.
     * @param args Arguments to this callback.
     * @param output Output variant.
     * @return Return code representing the success or failure of this callback's execution.
     */
    COMMANDSTORESHARED_EXPORT int ccb_Echo(const CommandSenderInfo &info, const QStringList &args, QVariant &output);
    
    /**
     * @brief Outputs messages to test output types and colours for the current style.
     * @param info Information about the calling command manager.
     * @param args Arguments to this callback.
     * @param output Output variant.
     * @return Return code representing the success or failure of this callback's execution.
     */
    COMMANDSTORESHARED_EXPORT int ccb_Con_TestColours(const CommandSenderInfo &info, const QStringList &args, QVariant &output);
}

#endif // COMMANDSTORE_H
