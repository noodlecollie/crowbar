#include "wr_commandentrybox.h"
#include <QWheelEvent>

CommandEntryBox::CommandEntryBox(QWidget *parent) :
    QLineEdit(parent), m_pInterpreter(NULL), m_pSuggestions(NULL)
{
}

CommandEntryBox::CommandEntryBox(CommandInterpreter *interp, QWidget *parent) :
    QLineEdit(parent), m_pInterpreter(interp), m_pSuggestions(NULL)
{
}

void CommandEntryBox::sendCommandString()
{
    if ( !m_pInterpreter ) return;
    
    // Strip any preceding or succeeding whitespace from the content
    // and send it to the interpreter.
    m_pInterpreter->parse(text().trimmed());
    
    // Clear the text box.
    clear();
    
    // Clear and hide the suggestions box just in case.
    if ( m_pSuggestions )
    {
        m_pSuggestions->clear();
        m_pSuggestions->hide();
    }
}

void CommandEntryBox::setInterpreter(CommandInterpreter *interp)
{
    m_pInterpreter = interp;
}

CommandInterpreter* CommandEntryBox::getInterpreter() const
{
    return m_pInterpreter;
}

CommandSuggestionList* CommandEntryBox::getSuggestionList() const
{
    return m_pSuggestions;
}

void CommandEntryBox::setSuggestionList(CommandSuggestionList *list)
{
    m_pSuggestions = list;
}

bool CommandEntryBox::insertSuggestion()
{
    if ( !suggestionsValid() ) return false;
    
    if ( !m_pSuggestions->hasSelection() ) m_pSuggestions->selectFirst();
    
    QString command = m_pSuggestions->getCurrentSelection();
    if ( command == "" ) return false;
    
    // If the entry box's text is the same as the command we're suggesting, we don't need to do anything.
    if ( text().trimmed() == command ) return true;
    
    // Remove all the characters we have entered so far.
    QRegularExpression regex(QRegularExpression::escape(text()));
    command.remove(regex);
    
    // Append the remaining characters.
    setText(text() + command);
    
    // Clear and hide the suggestions box.
    //m_pSuggestions->clear();
    //m_pSuggestions->hide();
    return true;
}

bool CommandEntryBox::replaceWithSuggestion()
{
    if ( !suggestionsValid() ) return false;
    
    //if ( !m_pSuggestions->hasSelection() ) m_pSuggestions->selectFirst();
    
    QString command = m_pSuggestions->getCurrentSelection();
    if ( command == "" ) return false;
    
    // Replace all the content with the current suggestion.
    setText(command);
    return true;
}

bool CommandEntryBox::suggestionsValid()
{
    return ( m_pSuggestions && !m_pSuggestions->isHidden() && m_pSuggestions->count() > 0 );
}

void CommandEntryBox::keyPressEvent(QKeyEvent *e)
{
    // Handle key presses which should complete the current command.
    if ( e->key() == Qt::Key_Tab || e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter )
    {
        // If we're unable to access the suggestions list, submit the command.
        if ( !suggestionsValid() )
        {
            sendCommandString();
        }
        // If the insertion was successful, accept the key press as handled and return.
        else if ( insertSuggestion() )
        {
            m_pSuggestions->clear();
            m_pSuggestions->hide();
            e->accept();
            return;
        }
    }
    else if ( e->key() == Qt::Key_Up )
    {
        // Move the selection up in the suggestions box.
        if ( suggestionsValid() )
        {
            m_pSuggestions->moveUp();
            replaceWithSuggestion();
            e->accept();
            return;
        }
    }
    else if ( e->key() == Qt::Key_Down )
    {
        // Move the selection down in the suggestions box.
        if ( suggestionsValid() )
        {
            m_pSuggestions->moveDown();
            replaceWithSuggestion();
            e->accept();
            return;
        }
    }
    
    // Base event first.
    QLineEdit::keyPressEvent(e);
    
    // Only continue if we are linked to a suggestions list and command interpreter.
    if ( !m_pSuggestions || !m_pInterpreter ) return;
    
    // If there is more than one argument, don't do a suggestions list for now.
    // We should probably advance this later to search for possible valid args, etc.
    QStringList list;  
    QRegularExpressionMatchIterator m = CommandInterpreter::matchArgsStrict.globalMatch(text());
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
    m_pInterpreter->getSuggestions(list.at(0), suggestions);
    
    // Clear the current suggestions list.
    m_pSuggestions->clear();
    
    // If we got no suggestions back, exit.
    if ( suggestions.count() < 1 )
    {
        m_pSuggestions->hide();
        return;
    }
    
    // Add the suggestions to the list.
    // TODO: Have each entry contain an item specifying whether it's a command or a variable.
    foreach(CommandInterpreter::CommandIdentPair p, suggestions)
    {
        m_pSuggestions->addItem(p.second);
    }
    
    // Sort them alphabetically.
    m_pSuggestions->sortItems();
    
    // Show and raise the suggestions list.
    repositionSuggestions();
    m_pSuggestions->show();
    //m_pSuggestions->raise();
    
    // Select the first item.
    //m_pSuggestions->selectFirst();
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
    
    if ( !m_pSuggestions ) return;
    
    // Move the selection up or down the suggestions box.
    // Scroll down is negative in Y, up is positive.
    bool scrollDown = (e->angleDelta().y() < 0);
    int degrees = e->angleDelta().y() < 0 ? -(e->angleDelta().y()) : e->angleDelta().y();
    
    // Keep taking away multiples of 120 to find out how many scrolls we need to do.
    while ( degrees >= 120 )
    {
        scrollDown ? m_pSuggestions->moveDown() : m_pSuggestions->moveUp();
        degrees -= 120;
    }
}

void CommandEntryBox::repositionSuggestions()
{
    if ( !m_pSuggestions ) return;
    
    QPoint p = mapToGlobal(QPoint(0,height()+2));
    m_pSuggestions->move(p);
}
