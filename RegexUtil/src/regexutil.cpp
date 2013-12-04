#include "regexutil.h"
#include <QString>

namespace RegexUtil
{
    const REGEXUTILSHARED_EXPORT QString RegexMatchCommandArgs = QString("\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\"|[\\S]+");
    const REGEXUTILSHARED_EXPORT QString RegexMatchCommandArgsWs = QString("\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\"|[\\S]+|[\\s]+");
    const REGEXUTILSHARED_EXPORT QString RegexMatchDelimPipe = QString("\\s*(?<!\\\\)\\|\\s*");
    const REGEXUTILSHARED_EXPORT QString RegexMatchDelimSemicolon = QString("\\s*(?<!\\\\)\\;\\s*");
    const REGEXUTILSHARED_EXPORT QString RegexMatchLastArgNonWs = QString("\\S*$");
    const REGEXUTILSHARED_EXPORT QString RegexMatchLastArgWord = QString("\\w*$");
}
