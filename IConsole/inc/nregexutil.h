/** \file nregexutil.h
 * \brief Defines useful QRegularExpression objects.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef NREGEXUTIL_H
#define NREGEXUTIL_H

class QString;

// TODO: Put this into its own library!

namespace RegexUtil
{
    /**
     * @brief Regex to match command arguments.
     * Arguments are either space-delimited or enclosed in unescaped quotes.<br/>
     * <b>\verbatim "[^"\\]*(?:\\.[^"\\]*)*"|[\S]+ \endverbatim </b><br/>
     * For example, when parsing the following string:<br/>
     * <b>this is a \" test\" "command \"string\""</b><br/>
     * The matches are:<br/>
     * <ul><li>this</li>
     * <li>is</li>
     * <li>a</li>
     * <li>\"</li>
     * <li>test\"</li>
     * <li>"command \"string\""</li></ul>
     */
    extern const QString RegexMatchCommandArgs;
    
    /**
     * @brief Regex to match command arguments but also spaces between arguments.<br/>
     * <b>\verbatim "[^"\\]*(?:\\.[^"\\]*)*"|[\S]+|[\s]+ \endverbatim</b><br/>
     * Same as the non-strict regex except spaces between the arguments are also matched.
     * This allows checking whether a user has finished typing the first argument in a given
     * string (ie. the command name) and is going to type any subsequent arguments for that command.
     * As soon as the number of matches is greater than 1, the user has stopped typing the command name.
     */
    extern const QString RegexMatchCommandArgsWs;
    
    /**
     * @brief Regex to match pipe characters surrounded by whitespace.
     * Will match unescaped pipe characters, regardless of how much whitespace they are (or are not) surrounded by.<br/>
     * <b>\verbatim \s*(?<!\\)\|\s* \endverbatim </b><br/>
     * Searches for 0 or more whitespace characters, followed by a pipe which is not preceded by a backslash (using negative
     * lookbehind), followed by 0 or more whitespace characters.
     */
    extern const QString RegexMatchDelimPipe;
    
    /**
     * @brief Regex to match semicolons surrounded by whitespace.<br/>
     * <b>\verbatim \s*(?<!\\)\;\s* \endverbatim </b><br/>
     * Similar construction to the pipe regex.
     */
    extern const QString RegexMatchDelimSemicolon;
    
    /**
     * @brief Regex to match the last argument in a command string.<br/>
     * <b>\verbatim \S*$ \endverbatim </b><br/>
     * Matches any non-whitespace character any number of times at the end of the string.
     * If there are no non-whitespace characters (but there is whitespace), does not match.
     */
    extern const QString RegexMatchLastArgNonWs;
    
    /**
     * @brief Regex to match the last argument in a command string.</br>
     * <b>\verbatim \w*$ \endverbatim </b><br/>
     * In contrast to the above (which will match any non-whitespace character), this regex
     * will match only letters, numbers and underscores.
     */
    extern const QString RegexMatchLastArgWord;
}

#endif // NREGEXUTIL_H

/**@}*/
