#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QList>
#include <QPair>

#include "consolewidget.h"

// TEMP
#include <QtDebug>
// TEMP

#define IMPLEMENT_CUSTOM_COLOUR(_col) \
QColor ConsoleWidget::customColor##_col() const \
{ \
    return m_colCustom##_col; \
} \
void ConsoleWidget::setCustomColor##_col(QColor col) \
{ \
    m_colCustom##_col = col; \
    changeBlockColours(ANCHOR_CUSTOM##_col, m_colCustom##_col); \
} \
void ConsoleWidget::resetCustomColor##_col() \
{ \
    setCustomColor##_col(DEFAULT_CUSTOM_COLOUR##_col); \
}

const QColor ConsoleWidget::DEFAULT_WARNING_COLOUR = QColor(255,0,0);
const QColor ConsoleWidget::DEFAULT_MESSAGE_COLOUR = QColor(0,0,0);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR1 = QColor(51,181,48);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR2 = QColor(58,107,255);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR3 = QColor(255,106,43);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR4 = QColor(206,206,206);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR5 = QColor(0,255,255);
const QColor ConsoleWidget::DEFAULT_CUSTOM_COLOUR6 = QColor(255,0,255);
const unsigned int ConsoleWidget::DEFAULT_MAX_LINES = 256;
const QString ConsoleWidget::ANCHOR_MESSAGE = QString("anchor_message");
const QString ConsoleWidget::ANCHOR_WARNING = QString("anchor_warning");
const QString ConsoleWidget::ANCHOR_CUSTOM1 = QString("anchor_custom1");
const QString ConsoleWidget::ANCHOR_CUSTOM2 = QString("anchor_custom2");
const QString ConsoleWidget::ANCHOR_CUSTOM3 = QString("anchor_custom3");
const QString ConsoleWidget::ANCHOR_CUSTOM4 = QString("anchor_custom4");
const QString ConsoleWidget::ANCHOR_CUSTOM5 = QString("anchor_custom5");
const QString ConsoleWidget::ANCHOR_CUSTOM6 = QString("anchor_custom6");

// Implement custom colours
IMPLEMENT_CUSTOM_COLOUR(1)
IMPLEMENT_CUSTOM_COLOUR(2)
IMPLEMENT_CUSTOM_COLOUR(3)
IMPLEMENT_CUSTOM_COLOUR(4)
IMPLEMENT_CUSTOM_COLOUR(5)
IMPLEMENT_CUSTOM_COLOUR(6)

ConsoleWidget::ConsoleWidget(QWidget *parent) : QTextEdit(parent),
    m_colWarning(DEFAULT_WARNING_COLOUR),
    m_colMessage(DEFAULT_MESSAGE_COLOUR),
    m_colCustom1(DEFAULT_CUSTOM_COLOUR1),
    m_colCustom2(DEFAULT_CUSTOM_COLOUR2),
    m_colCustom3(DEFAULT_CUSTOM_COLOUR3),
    m_colCustom4(DEFAULT_CUSTOM_COLOUR4),
    m_colCustom5(DEFAULT_CUSTOM_COLOUR5),
    m_colCustom6(DEFAULT_CUSTOM_COLOUR6),
    m_iMaxLines(DEFAULT_MAX_LINES)
{
    setReadOnly(true);
    setUndoRedoEnabled(false);
    setWordWrapMode(QTextOption::WrapAnywhere);
    setLineWrapMode(QTextEdit::WidgetWidth);
}

void ConsoleWidget::setWarningColor(QColor warningColor)
{
    m_colWarning = warningColor;
    changeBlockColours(ANCHOR_WARNING, m_colWarning);
}

QColor ConsoleWidget::warningColor() const
{
    return m_colWarning;
}

void ConsoleWidget::resetWarningColor()
{
    setWarningColor(DEFAULT_WARNING_COLOUR);
}

QColor ConsoleWidget::messageColor() const
{
   return m_colMessage; 
}

void ConsoleWidget::setMessageColor(QColor messageColor)
{
    m_colMessage = messageColor;
    changeBlockColours(ANCHOR_MESSAGE, m_colMessage);
}

void ConsoleWidget::resetMessageColor()
{
    setMessageColor(DEFAULT_MESSAGE_COLOUR);
}

void ConsoleWidget::printWarning(const QString &message)
{
    // BUG!! moveCursor resets text colour after moving, so it's here for now until this gets resolved.
//    moveCursor(QTextCursor::End);
//    setTextColor(m_colWarning);
//    print(message);
//    setTextColor(m_colMessage);
    printMessage(CommandSenderInfo::OutputWarning, message);
}

void ConsoleWidget::printMessage(const QString &message)
{
    // BUG!! moveCursor resets text colour after moving, so it's here for now until this gets resolved.
//    moveCursor(QTextCursor::End);
//    setTextColor(m_colMessage);
//    print(message);
    printMessage(CommandSenderInfo::OutputGeneral, message);
}

void ConsoleWidget::print(const QString &msg, const QString* anchor)
{
    //qDebug() << "Font:" << fontFamily();
    QString msg2 = msg.toHtmlEscaped();
    msg2.replace('\n', "<br />");
    QString toInsert;
    
    if ( anchor )
    {
        toInsert = QString("<span style=\"color: %0\"><a name=\"%1\"></a>%2</span>").arg(textColor().name()).arg(*anchor).arg(msg2);
    }
    else
    {
        toInsert = msg2;
    }
    
    insertHtml(toInsert);
    trimToLineLimit();
    ensureCursorVisible();
    
    //qDebug() << "Content:" << toHtml();
}

//void ConsoleWidget::printCustom(CommandSenderInfo::OutputType type, const QString &msg)
//{
//    // BUG!! moveCursor resets text colour after moving, so it's here for now until this gets resolved.
//    moveCursor(QTextCursor::End);
    
//    switch(type)
//    {
//        case CommandSenderInfo::OutputCustom1: setTextColor(m_colCustom1); break;
//        case CommandSenderInfo::OutputCustom2: setTextColor(m_colCustom2); break;
//        case CommandSenderInfo::OutputCustom3: setTextColor(m_colCustom3); break;
//        case CommandSenderInfo::OutputCustom4: setTextColor(m_colCustom4); break;
//        case CommandSenderInfo::OutputCustom5: setTextColor(m_colCustom5); break;
//        case CommandSenderInfo::OutputCustom6: setTextColor(m_colCustom6); break;
//        default:                               setTextColor(m_colMessage); break;
//    }
    
//    print(msg);
//}

void ConsoleWidget::printMessage(CommandSenderInfo::OutputType type, const QString &message)
{
//    QColor col1 = textColor();
//    QColor col2 = messageColor();
//    qDebug() << col1.red() << col1.green() << col1.blue() << col2.red() << col2.green() << col2.blue();
    
    moveCursor(QTextCursor::End);
    //QTextCharFormat f;
    //f.setAnchor(true);
    QString anchorName;
    
    switch(type)
    {
        case CommandSenderInfo::OutputWarning:
        {
            anchorName = ANCHOR_WARNING;
            setTextColor(m_colWarning);
            break;
        }
        case CommandSenderInfo::OutputCustom1:
        {
            anchorName = ANCHOR_CUSTOM1;
            setTextColor(m_colCustom1);
            break;
        }
        case CommandSenderInfo::OutputCustom2:
        {
            anchorName = ANCHOR_CUSTOM2;
            setTextColor(m_colCustom2);
            break;
        }
        case CommandSenderInfo::OutputCustom3:
        {
            anchorName = ANCHOR_CUSTOM3;
            setTextColor(m_colCustom3);
            break;
        }
        case CommandSenderInfo::OutputCustom4:
        {
            anchorName = ANCHOR_CUSTOM4;
            setTextColor(m_colCustom4);
            break;
        }
        case CommandSenderInfo::OutputCustom5:
        {
            anchorName = ANCHOR_CUSTOM5;
            setTextColor(m_colCustom5);
            break;
        }
        case CommandSenderInfo::OutputCustom6:
        {
            anchorName = ANCHOR_CUSTOM6;
            setTextColor(m_colCustom6);
            break;
        }
        default:
        {
            anchorName = ANCHOR_MESSAGE;
            setTextColor(m_colMessage);
            break;
        }
    }
    
    print(message, &anchorName);
    
//    qDebug() << "After:" << col1.red() << col1.green() << col1.blue() << col2.red() << col2.green() << col2.blue();
}

int ConsoleWidget::maxLines() const
{
    return m_iMaxLines;
}

void ConsoleWidget::setMaxLines(int lines)
{
    m_iMaxLines = lines;
    trimToLineLimit();
}

void ConsoleWidget::resetMaxLines()
{
    setMaxLines(DEFAULT_MAX_LINES);
}

void ConsoleWidget::trimToLineLimit()
{
    // When the max block limit is set on the underlying QTextDocument, any lines exceeding the limit will be
    // trimmed from the top of the document.
    
    QTextDocument* doc = document();
    if ( doc && m_iMaxLines >= 1 && doc->blockCount() > m_iMaxLines )   // No point if m_iMaxLines is 0, since blockCount should never be 0.
    {
        doc->setMaximumBlockCount(m_iMaxLines);
        doc->setMaximumBlockCount(0);
    }
}

void ConsoleWidget::changeBlockColours(const QString &name, const QColor &col)
{
    /*
     * Problem: Messages can be output to the ConsoleWidget before the application of a style sheet has taken place.
     * Because messages are printed in arbitrary colours depending on their type, the colour of messages previously printed
     * does not change when a new style sheet is applied, making them unreadable if the background colour is changed to be
     * the same colour as some text, for example.
     *
     * Proposed solution:
     * Each line ("block") in the underlying QTextEdit is atomic in that it is either a standard message, warning or custom
     * colour. Each of these categories has a QColor associated with it. When a message of any kind is written to the QTextEdit,
     * <span></span> tags are placed manually around the message. The span tag contains the colour of the message as well as
     * the class - message/warning/custom1-6. When a QColor property is changed (as it will be on the application of a style sheet),
     * the currently held text is parsed and all references to the class corresponding to this property are found; the colour of the
     * font tag is changed and the HTML re-added to the QTextEdit.
     *
     * More problems (after investigation):
     * Qt is a massive pain in the arse with its handling of HTML: it will automatically parse, escape and modify the HTML you insert
     * and has no way of inserting raw characters (as far as I can currently see) without them being escaped. Class attributes are
     * completely removed from span tags. Hence, the following system applies:
     * Messages have <a name="anchor_name"></a> before them, where anchor_name is unique to the message type.
     * Messages are surrounded by <span style=" color:#xxxxxx"></span> tags. There may well be more than one span tag surrounding
     * different parts - Qt likes to separate capital letters and put them in their own span tags.
     * Any text after the anchor tag relates to that category of text, until the end of the file or another anchor tag is reached.
     */
    
    QTextDocument* doc = document();
    if ( !doc ) return;
    
    QString content = toHtml();
    //qDebug() << "Content:" << content;
    
    QList<QPair<int, int>> anchorIndices;
    QRegularExpression regexAnchor("<a name=\"\\w*\"><\\/a>");
    
    // Run match. If a match occurred, store index of beginning and length of match and run match again from new offset.
    int lastIndex = 0;
    int lastLength = 0;
    QRegularExpressionMatch match;
    
    while ( (match = regexAnchor.match(content, lastIndex + lastLength)).hasMatch() )
    {
        // Store index of the point at which the match was found.
        lastIndex = match.capturedStart();
        lastLength = match.capturedLength();
        anchorIndices.append(QPair<int, int>(lastIndex, lastLength));
        //qDebug() << "Found a match at index" << lastIndex << "of length" << lastLength << ":" << content.mid(lastIndex, lastLength);
    }
    
    // Find the last occurrence of </body>.
    // This means that the last entry in the anchorIndices list is the entry for the body end tag.
    // We set the second value of the pair to -1.
    // This way, we'll be able to pick out the index that the body end tag begins at and search only up to this index.
    QRegularExpression regexBodyTerminator("<\\/body");
    QRegularExpressionMatch matchBody = regexBodyTerminator.match(content);
    if ( matchBody.hasMatch() )
    {
        anchorIndices.append(QPair<int, int>(matchBody.capturedStart(), -1));
    }
    
    // anchorIndices now contains references to all anchors found in the document.
    // Go through each anchor and consider it if the anchor name is the same as the name we were passed in.
    for( int i = 0; i < anchorIndices.length(); i++ )
    {
        QPair<int, int> pair = anchorIndices.at(i);
        
        // If second is -1, this means this is the body tag. Exit here.
        if ( pair.second == -1 )
        {
            break;
        }
        
        // Get the anchor string.
        QString anchor = content.mid(pair.first, pair.second);
        
        // Find the name attribute.
        QRegularExpression nameAtt("name=\"");
        QRegularExpressionMatch m = nameAtt.match(anchor);
        if ( !m.hasMatch() )
        {
            continue;
        }
        
        // Find the index of the first character after 'name="'.
        int start = m.capturedStart() + m.capturedLength();
        
        // Find the index of the first quote mark after this index.
        int end = anchor.indexOf('"', start);
        
        // Retrieve the name of the anchor.
        QString anchorName = anchor.mid(start, (end >= 0) ? end - start : anchor.length() - start);
        //qDebug() << "Name of anchor:" << anchorName;
        
        if ( anchorName.isEmpty() || anchorName.compare(name) != 0 ) continue;
        
        // The name is the same.
        // Go through the content up to the index of the next anchor and check all span tags.
        int substringStart = pair.first + pair.second;
        
        // We should never reach the last index here as it will be the body tag, but for the sake of safety:
        int substringEnd = (i == anchorIndices.length() - 1) ? content.length() - 1 : anchorIndices.at(i+1).first - 1;
        QRegularExpression regexColorAtt("<span style=\"[^\\\"]*color:#");
        
        // Substring holds the section of the main content that we are searching.
        QString substring = content.mid(substringStart, substringEnd - substringStart + 1);
        //qDebug() << "Substring ranges from" << substringStart << "to" << substringEnd;
        //qDebug() << substring;
        
        // Last holds the index to begin the next search from.
        int last = 0;
        QRegularExpressionMatch match2;
        
        while ( (match2 = regexColorAtt.match(substring, last)).hasMatch() )
        {
            int colStart = match2.capturedEnd()-1;              // Where colour starts in substring
            const int colLength = 7;                            // Length of colour string
            int contentColStart = substringStart + colStart;    // Where colour starts in content
            
            //qDebug() << "Match begin:" << match2.capturedStart() << "Match end:" << match2.capturedEnd();
            //qDebug() << "Colour string:" << substring.mid(colStart, colLength);
            //qDebug() << "Colour string in content:" << content.mid(contentColStart, colLength);
            //qDebug() << "Replacing with:" << col.name();
            //substring.replace(colStart, colLength, col.name());
            content.replace(contentColStart, colLength, col.name());
            //qDebug() << "New colour string:" << substring.mid(colStart, colLength);
            //qDebug() << "New colour string in content:" << content.mid(contentColStart, colLength);
            
            last = colStart + colLength;
            //qDebug() << "New index to search from:" << last;
        }
    }
    
    // Replace the content of the QTextEdit with the content string.
    setHtml(content);
}
