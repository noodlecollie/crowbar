/*! \file commandlineparser.h
 * \brief Defines the class responsible for parsing command-line arguments and setting the relevant settings in response.
 */

#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QObject>

/**
 * @brief Deals with arguments passed to the application on the command-line.
 *
 * The command-line parser is created before the main application window is initialised and parses any options sent to the
 * application on the command line. The global instance of the parser is made available through globals.h under g_pCmdLine
 * and several macros exist for facilitating the checking of common global application properties. See globals.h for a full description.
 */
class CommandLineParser : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Constructor.
     * @param parent Parent object (usually NULL).
     */
    explicit CommandLineParser(QObject *parent = 0);
    
signals:
    
public slots:

    /**
     * @brief Parses the command-line arguments from the ignition function.
     * @param argc Number of arguments.
     * @param argv Char array of arguments.
     */
    void ParseArguments(int argc, char **argv);

private:
};

#endif // COMMANDLINEPARSER_H
