#include "nglobalcmd.h"

namespace NGlobalCmd
{
    QString commandFlagsToString(CMDFLAGS cmdFlags) 
    {
        bool printed = false;
        QString output;
        
        if ( (cmdFlags & CMDFLAG_READONLY) == CMDFLAG_READONLY )
        {
            if ( printed ) output.append("|");
            output.append(FLAGNAMES[0]);
            printed = true;
        }
        
        if ( (cmdFlags & CMDFLAG_ENSURECALLBACK) == CMDFLAG_ENSURECALLBACK )
        {
            if ( printed ) output.append("|");
            output.append(FLAGNAMES[1]);
            printed = true;
        }
        
        return output;
    }
}
