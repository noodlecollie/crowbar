/*! \file commandlineparser.h
 * \brief Defines the class responsible for parsing command-line arguments and setting the relevant settings in response.
 */

#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

//! \def CLA_DEBUGGING
//! \brief Command-line option string to enable debugging.
#define CLA_DEBUGGING   "-debug"

//! \def CLA_LOGGING
//! \brief Command-line option string to enable logging.
#define CLA_LOGGING     "-log"

#include <QObject>

// TODO: Change this to set console variables instead.

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

    /**
     * @brief Is the application in debug mode?
     * @note If enabled, the log window is available from the main application Debug menu.
     *
     * @note If not specified, debugging mode defaults to false.
     * @return True if in debug mode, false otherwise.
     */
    bool Debugging();

    /**
     * @brief Is logging mode enabled?
     * @note If enabled, logging messsages are written to a log file. If in debug mode, logging mode is also enabled.
     *
     * @note If not specified, logging mode defaults to false.
     * @return True if enabled, false otherwise.
     */
    bool Logging();
    
signals:
    
public slots:

    /**
     * @brief Parses the command-line arguments from the ignition function.
     * @param argc Number of arguments.
     * @param argv Char array of arguments.
     */
    void ParseArguments(int argc, char **argv);

private:
    bool m_bDebugging;  /**< Whether debug mode is enabled. Defaults to false. */
    bool m_bLogging;    /**< Whether logging mode is enabled. Defaults to false. */
};

#endif // COMMANDLINEPARSER_H
