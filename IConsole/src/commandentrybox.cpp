#include "wr_commandentrybox.h"
#include <QWheelEvent>
#include <QFocusEvent>
#include <QListWidgetItem>
#include <QIcon>
#include <QApplication>
#include <QDir>

#include "regexutil.h"

// TEMP
#include <QtDebug>
// TEMP

const QString CommandEntryBox::LI_NAME_COMMAND = "CommandListItem";
const QString CommandEntryBox::LI_NAME_VARIABLE = "VariableListItem";
const int CommandEntryBox::DEFAULT_COMMAND_HISTORY_SIZE = 32;

CommandEntryBox::CommandEntryBox(QWidget *parent) :
    QLineEdit(parent), m_pSuggestions(new CommandSuggestionList(this)), m_szIconConCommand(), m_szIconConVar(), m_bShouldGetSuggestions(true),
    m_CommandHistory(), m_iCommandHistorySize(DEFAULT_COMMAND_HISTORY_SIZE), m_iCurrentHistoryIndex(-1)
{
    // Connect up our internal signals/slots.
    connect(this, SIGNAL(returnPressed()), SLOT(sendCommandString()));
    connect(this, SIGNAL(upArrowPressed()), SLOT(chooseAboveSuggestion()));
    connect(this, SIGNAL(downArrowPressed()), SLOT(chooseBelowSuggestion()));
    connect(this, SIGNAL(mouseWheel(int)), SLOT(scrollSuggestionSelection(int)));
    connect(this, SIGNAL(tabPressed()), SLOT(completeWithCurrentSuggestion()));
    connect(this, SIGNAL(textChanged(QString)), SLOT(processForSuggestions(QString)));
    
    // Set up suggestions box.
    if ( m_pSuggestions )
    {
        m_pSuggestions->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        m_pSuggestions->setMaximumHeight(200);
        m_pSuggestions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pSuggestions->setUniformItemSizes(true);
        m_pSuggestions->hide();
        connect(m_pSuggestions, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(completeListItem(QListWidgetItem*)));
    }
}

int CommandEntryBox::commandHistorySize() const
{
    return m_iCommandHistorySize;
}

void CommandEntryBox::setCommandHistorySize(int size)
{
    m_iCommandHistorySize = size;
    m_iCurrentHistoryIndex = -1;
    
    // If size is negative or 0, clear the command list.
    if ( size <= 0 )
    {
        m_CommandHistory.clear();
        return;
    }
    
    // If we have too many stored commands:
    while ( size < m_CommandHistory.size() )
    {
        m_CommandHistory.removeLast();
    }
}

void CommandEntryBox::resetCommandHistorySize()
{
    setCommandHistorySize(DEFAULT_COMMAND_HISTORY_SIZE);
}

QString CommandEntryBox::iconConCommand() const
{
    return m_szIconConCommand;
}

void CommandEntryBox::setIconConCommand(QString icon)
{
    m_szIconConCommand = icon;
}

void CommandEntryBox::resetIconConCommand()
{
    m_szIconConCommand = QString("");
}

QString CommandEntryBox::iconConVar() const
{
    return m_szIconConVar;
}

void CommandEntryBox::setIconConVar(QString icon)
{
    m_szIconConVar = icon;
}

void CommandEntryBox::resetIconConVar()
{
    m_szIconConVar = QString("");
}

void CommandEntryBox::sendCommandString()
{
    // If we will exceed the number of commands to hold in history, remove from the head.
    while ( m_CommandHistory.size() >= m_iCommandHistorySize )
    {
        m_CommandHistory.removeLast();
    }
    
    // Store the command string at the head of the list.
    QString command = text().trimmed();
    m_CommandHistory.prepend(command);
    m_iCurrentHistoryIndex = -1;
    
    // Send the string.
    emit commandString(command);
    
    // Clear the text box.
    clear();
    
    // Clear and hide the suggestions box just in case.
    if ( m_pSuggestions )
    {
        m_pSuggestions->clear();
        m_pSuggestions->hide();
    }
}

const CommandSuggestionList* CommandEntryBox::getSuggestionList() const
{
    return m_pSuggestions;
}

//void CommandEntryBox::setSuggestionList(CommandSuggestionList *list)
//{
//    m_pSuggestions = list;
//}

bool CommandEntryBox::replaceWithSuggestion(bool shouldRequery)
{
    if ( !suggestionsValid() ) return false;
    
    if ( !m_pSuggestions->hasSelection() ) m_pSuggestions->selectFirst();
    
    QString command = m_pSuggestions->getCurrentSelection();
    if ( command == "" ) return false;
    
    // Replace all the content with the current suggestion.
    m_bShouldGetSuggestions = shouldRequery;
    setText(command);
    m_bShouldGetSuggestions = true;
    
    return true;
}

bool CommandEntryBox::insertSuggestion(bool shouldRequery)
{
    if ( !suggestionsValid() ) return false;
    
    if ( !m_pSuggestions->hasSelection() ) m_pSuggestions->selectFirst();
    
    QString command = m_pSuggestions->getCurrentSelection();
    if ( command == "" ) return false;
    
    // Hold the last argument we typed.
    QString lastArg;
    QRegularExpression matchLastArg("\\S*$");
    QRegularExpressionMatch m = matchLastArg.match(text());
    
    // If we have a last argument
    if ( m.hasMatch() )
    {
        lastArg = m.captured();
    }
    // We have no last argument - set lastArg to blank.
    else
    {
        lastArg = "";
    }
    
    QString command_pre = command;
    if ( !lastArg.isEmpty() )
    {
        // Remove all the characters we have entered so far.
        // This is assuming that the current last argument is actually part of the suggestion!
        QRegularExpression regex("^" + QRegularExpression::escape(text()));
        command.remove(regex);
        
        Q_ASSERT_X(command_pre != command, "CommandEntryBox::insertSuggestion()",
                   "Command was erroneously suggested for most recent argument in entry box.");
    }
    
    // Append the remaining characters.
    m_bShouldGetSuggestions = shouldRequery;
    setText(text() + command);
    m_bShouldGetSuggestions = true;
    
    return true;
}

bool CommandEntryBox::suggestionsValid()
{
    return ( m_pSuggestions && !m_pSuggestions->isHidden() && m_pSuggestions->count() > 0 );
}

void CommandEntryBox::keyPressEvent(QKeyEvent *e)
{
    // Base event first.
    QLineEdit::keyPressEvent(e);
    
    // Fire signals depending on what key was pressed.
    switch (e->key())
    {
        case Qt::Key_Tab:
        {
            emit tabPressed();
            break;
        }
        
        case Qt::Key_Up:
        {
            emit upArrowPressed();
            break;
        }
        
        case Qt::Key_Down:
        {
            emit downArrowPressed();
            break;
        }
    }
}

void CommandEntryBox::clearCommandHistory()
{
    m_CommandHistory.clear();
    m_iCurrentHistoryIndex = -1;
}

void CommandEntryBox::processForSuggestions(const QString &str)
{
    // Only continue if we are linked to a suggestions list.
    if ( !m_pSuggestions ) return;
    
    // If we shouldn't be getting suggestions, exit.
    if ( !m_bShouldGetSuggestions ) return;
    
    // If there is more than one argument, don't do a suggestions list for now.
    // We should probably advance this later to search for possible valid args, etc.
    QStringList list;
    QRegularExpression matchArgsWithWhitespace(NRegexUtil::RegexMatchCommandArgsWs);
    QRegularExpressionMatchIterator m = matchArgsWithWhitespace.globalMatch(QRegularExpression::escape(str));
    while ( m.hasNext() )
    {
        list.append(m.next().captured(0));
    }
    
    if ( list.count() != 1 )
    {
        m_pSuggestions->clear();
        m_pSuggestions->hide();
        return;
    }
    
    // Get suggestions for the current prefix.
    QList<CommandInterpreter::CommandIdentPair> suggestions;
    emit getSuggestions(list.at(0), suggestions, -1);
    
    // Clear the current suggestions list.
    m_pSuggestions->clear();
    
    // If we got no suggestions back, exit.
    if ( suggestions.count() < 1 )
    {
        m_pSuggestions->hide();
        return;
    }
    
    // Add the suggestions to the list.
    foreach(CommandInterpreter::CommandIdentPair p, suggestions)
    {
        QDir dir(qApp->applicationDirPath());
        QString iconPath;

        switch (p.first)
        {
            case NGlobalCmd::CICommand:
            {
                if ( !m_szIconConCommand.isEmpty() && dir.exists(m_szIconConCommand) )
                {
                    iconPath = dir.filePath(m_szIconConCommand);
                }
                
                break;
            }
            
            case NGlobalCmd::CIVariable:
            {
                if ( !m_szIconConVar.isEmpty() && dir.exists(m_szIconConVar) )
                {
                    iconPath = dir.filePath(m_szIconConVar);
                }
                
                break;
            }
        }
        
        QListWidgetItem* i = new QListWidgetItem(p.second);
        //i->setSizeHint(QSize(100, 20));
        
        if ( !iconPath.isEmpty() )
        {
            i->setIcon(QIcon(iconPath));
        }
        
        m_pSuggestions->addItem(i);
    }
    
    // Sort them alphabetically.
    m_pSuggestions->sortItems();
    
    // Show the suggestions list.
    repositionSuggestions();
    m_pSuggestions->autoWidth();
    m_pSuggestions->autoHeight();
    m_pSuggestions->show();
    m_pSuggestions->selectFirst();
}

void CommandEntryBox::moveEvent(QMoveEvent *e)
{
    // Base event first.
    QLineEdit::moveEvent(e);
    
    // Reposition our suggestions box.
    if ( m_pSuggestions && !m_pSuggestions->isHidden() ) repositionSuggestions();
}

void CommandEntryBox::hideEvent(QHideEvent *e)
{
    // Base event first.
    QLineEdit::hideEvent(e);
    
    // If we have a suggestions box, hide it.
    if ( m_pSuggestions ) m_pSuggestions->hide();
}

void CommandEntryBox::showEvent(QShowEvent *e)
{
    // Base event first.
    QLineEdit::showEvent(e);
    
    // If we have a suggestions box and it is not empty, show it.
    if ( m_pSuggestions && m_pSuggestions->count() > 0 ) m_pSuggestions->show();
}

void CommandEntryBox::wheelEvent(QWheelEvent *e)
{
    // Base event first.
    QLineEdit::wheelEvent(e);
    
    emit mouseWheel(e->angleDelta().y());
}

void CommandEntryBox::repositionSuggestions()
{
    if ( !m_pSuggestions ) return;
    
    QPoint p = mapToGlobal(QPoint(0,height()+2));
    m_pSuggestions->move(p);
}

void CommandEntryBox::moveSuggestionSelectionUp()
{
    if ( suggestionsValid() ) m_pSuggestions->moveUp();
}

void CommandEntryBox::moveSuggestionSelectionDown()
{
    if ( suggestionsValid() ) m_pSuggestions->moveDown();
}

void CommandEntryBox::chooseAboveSuggestion()
{
    // We have a suggestions box, move the selection instead.
    if ( suggestionsValid() )
    {
        m_iCurrentHistoryIndex = -1;
        moveSuggestionSelectionUp();
        replaceWithSuggestion(false);
    }
    // We have no suggestions box, traverse command history.
    else
    {
        traverseHistory(true);
    }
}

void CommandEntryBox::chooseBelowSuggestion()
{
    // We have a suggestions box, move the selection instead.
    if ( suggestionsValid() )
    {
        m_iCurrentHistoryIndex = -1;
        moveSuggestionSelectionDown();
        replaceWithSuggestion(false);
    }
    // We have no suggestions box, traverse command history.
    else
    {
        traverseHistory(false);
    }
}

void CommandEntryBox::traverseHistory(bool direction)
{
    if ( m_CommandHistory.size() < 1 ) return;
    
    // If direction = true, increase the counter.
    if ( direction )
    {
        if ( m_iCurrentHistoryIndex < 0 || m_iCurrentHistoryIndex >= m_CommandHistory.size()-1 ) m_iCurrentHistoryIndex = 0;
        else m_iCurrentHistoryIndex++;
    }
    // If direction = false, decrease the counter.
    else
    {
        if ( m_iCurrentHistoryIndex >= m_CommandHistory.size() || m_iCurrentHistoryIndex <= 0 ) m_iCurrentHistoryIndex = m_CommandHistory.size()-1;
        else m_iCurrentHistoryIndex--;
    }
    
    // Update the text box.
    m_bShouldGetSuggestions = false;
    setText(m_CommandHistory.at(m_iCurrentHistoryIndex));
    m_bShouldGetSuggestions = true;
}

void CommandEntryBox::completeWithCurrentSuggestion()
{
    insertSuggestion(); // Does a validity check.
    m_pSuggestions->clear();
    m_pSuggestions->hide();
}

void CommandEntryBox::scrollSuggestionSelection(int deg)
{
    bool isDown = (deg <= 0);
    if ( isDown ) deg = -deg;
    
    // Keep taking away multiples of 120 to find out how many scrolls we need to do.
    while ( deg >= 120 )
    {
        isDown ? moveSuggestionSelectionDown() : moveSuggestionSelectionUp();
        deg -= 120;
    }
    
    replaceWithSuggestion(false);
}

void CommandEntryBox::focusOutEvent(QFocusEvent *e)
{
    // Run base event.
    QLineEdit::focusOutEvent(e);
    
    // If we lost focus because of tab:
    if ( e->reason() == Qt::TabFocusReason )
    {
        // If we have text and an active suggestion:
        if ( !text().trimmed().isEmpty() && suggestionsValid() && !m_pSuggestions->getCurrentSelection().isEmpty() )
        {
            // Emit tab pressed.
            emit tabPressed();
            
            // HACK (a bit of one): Get our focus back!
            setFocus(Qt::OtherFocusReason);
        }
    }
}

void CommandEntryBox::completeListItem(QListWidgetItem *)
{
    completeWithCurrentSuggestion();
}
