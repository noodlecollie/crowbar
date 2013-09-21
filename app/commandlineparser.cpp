#include "commandlineparser.h"
#include <QString>
#include "globals.h"
#include "wr_convar.h"

CommandLineParser::CommandLineParser(QObject *parent) :
    QObject(parent)
{
}

void CommandLineParser::ParseArguments(int argc, char **argv)
{
    // Begin at i = 1 because arg 0 is the executable path.
//    for (int i = 1; i < argc; i++)
//    {
//        QString arg(argv[i]);

//        if ( arg.compare(CLA_DEBUGGING) == 0 )
//        {
//            // Debugging automatically enables logging too.
//            m_bDebugging = true;
//            m_bLogging = true;
//        }
//        else if ( arg.compare(CLA_LOGGING) == 0 )
//        {
//            m_bLogging = true;
//        }
//    }
}
