#include "wr_commandentrybox.h"
#include <QWheelEvent>
#include <QFocusEvent>
#include <QListWidgetItem>
#include <QIcon>
#include <QApplication>
#include <QDir>

const QString CommandEntryBox::LI_NAME_COMMAND = "CommandListItem";
const QString CommandEntryBox::LI_NAME_VARIABLE = "VariableListItem";

CommandEntryBox::CommandEntryBox(QWidget *parent) :
    QLineEdit(parent), m_pSuggestions(new CommandSuggestionList(this)), m_szIconConCommand(), m_szIconConVar()//, m_colBgCommand(), m_colBgVariable(),
    //m_bHasCmdCol(false), m_bHasVarCol(false)
{
    // Connect up our internal signals/slots.
    connect(this, SIGNAL(returnPressed()), SLOT(sendCommandString()));
    connect(this, SIGNAL(upArrowPressed()), SLOT(chooseAboveSuggestion()));
    connect(this, SIGNAL(downArrowPressed()), SLOT(chooseBelowSuggestion()));
    connect(this, SIGNAL(mouseWheel(int)), SLOT(scrollSuggestionSelection(int)));
    connect(this, SIGNAL(tabPressed()), SLOT(completeWithCurrentSuggestion()));
    connect(this, SIGNAL(textChanged(QString)), SLOT(processForSuggestions(QString)));
    
    if ( m_pSuggestions )
    {
        m_pSuggestions->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        m_pSuggestions->setMaximumHeight(200);
        m_pSuggestions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pSuggestions->setUniformItemSizes(true);
        m_pSuggestions->hide();
    }
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

//QColor CommandEntryBox::bgcolorConCommand() const
//{
//    return m_bHasCmdCol ? m_colBgCommand : QColor(255,255,255,0);
//}

//void CommandEntryBox::setBgcolorConCommand(QColor col)
//{
//    m_colBgCommand = col;
//    m_bHasCmdCol = true;
//}

//void CommandEntryBox::resetBgcolorConCommand()
//{
//    m_bHasCmdCol = false;
//}

//QColor CommandEntryBox::bgcolorConVar() const
//{
//    return m_bHasVarCol ? m_colBgVariable : QColor(255,255,255,0);
//}

//void CommandEntryBox::setBgcolorConVar(QColor col)
//{
//    m_colBgVariable = col;
//    m_bHasVarCol = true;
//}

//void CommandEntryBox::resetBgcolorConVar()
//{
//    m_bHasVarCol = false;
//}

void CommandEntryBox::sendCommandString()
{
    // Strip any preceding or succeeding whitespace from the content.
    emit commandString(text().trimmed());
    
    // Clear the text box.
    clear();
    
    // Clear and hide the suggestions box just in case.
    if ( m_pSuggestions )
    {
        m_pSuggestions->clear();
        m_pSuggestions->hide();
    }
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
    
    return true;
}

bool CommandEntryBox::replaceWithSuggestion()
{
    if ( !suggestionsValid() ) return false;
    
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
    // This is now all handled by signals/slots.
//    // Handle key presses which should complete the current command.
//    if ( e->key() == Qt::Key_Tab )
//    {
//        // If the insertion was successful, accept the key press as handled and return.
//        if ( insertSuggestion() )
//        {
//            m_pSuggestions->clear();
//            m_pSuggestions->hide();
//            e->accept();
//            return;
//        }
//    }
//    else if ( e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter )
//    {
//        sendCommandString();
//    }
//    else if ( e->key() == Qt::Key_Up )
//    {
//        // Move the selection up in the suggestions box.
//        if ( suggestionsValid() )
//        {
//            m_pSuggestions->moveUp();
//            replaceWithSuggestion();
//            e->accept();
//            return;
//        }
//    }
//    else if ( e->key() == Qt::Key_Down )
//    {
//        // Move the selection down in the suggestions box.
//        if ( suggestionsValid() )
//        {
//            m_pSuggestions->moveDown();
//            replaceWithSuggestion();
//            e->accept();
//            return;
//        }
//    }
    
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
    
    // Moved elsewhere as well.
//    // Only continue if we are linked to a suggestions list.
//    if ( !m_pSuggestions ) return;
    
//    // If there is more than one argument, don't do a suggestions list for now.
//    // We should probably advance this later to search for possible valid args, etc.
//    QStringList list;  
//    QRegularExpressionMatchIterator m = CommandInterpreter::matchArgsStrict.globalMatch(text());
//    while ( m.hasNext() )
//    {
//        list.append(m.next().captured(0));
//    }
    
//    if ( list.count() != 1 )
//    {
//        m_pSuggestions->clear();
//        m_pSuggestions->hide();
//        return;
//    }
    
//    // Get suggestions for the current prefix.
//    QList<CommandInterpreter::CommandIdentPair> suggestions;
//    //m_pInterpreter->getSuggestions(list.at(0), suggestions);
//    emit getSuggestions(list.at(0), suggestions, -1);
    
//    // Clear the current suggestions list.
//    m_pSuggestions->clear();
    
//    // If we got no suggestions back, exit.
//    if ( suggestions.count() < 1 )
//    {
//        m_pSuggestions->hide();
//        return;
//    }
    
//    // Add the suggestions to the list.
//    foreach(CommandInterpreter::CommandIdentPair p, suggestions)
//    {
//        QDir dir(qApp->applicationDirPath());
//        if ( dir.cd("resource") )
//        {
//            QIcon* icon = NULL;

//            switch (p.first)
//            {
//                case NGlobalCmd::CICommand:
//                {
//                    if ( dir.exists(command_img) )
//                    {
//                        icon = new QIcon(dir.filePath(command_img));
//                        break;
//                    }
//                }
                
//                case NGlobalCmd::CIVariable:
//                {
//                    if ( dir.exists(variable_img) )
//                    {
//                        icon = new QIcon(dir.filePath(variable_img));
//                        break;
//                    }
//                }
//            }
            
//            if ( icon )
//            {
//                QListWidgetItem* i = new QListWidgetItem(*icon, p.second);
//                m_pSuggestions->addItem(i);
//                continue;
//            }
//        }
        
//        m_pSuggestions->addItem(p.second);
//    }
    
//    // Sort them alphabetically.
//    m_pSuggestions->sortItems();
    
//    // Show the suggestions list.
//    repositionSuggestions();
//    m_pSuggestions->show();
}

void CommandEntryBox::processForSuggestions(const QString &str)
{
    // Only continue if we are linked to a suggestions list.
    if ( !m_pSuggestions ) return;
    
    // If there is more than one argument, don't do a suggestions list for now.
    // We should probably advance this later to search for possible valid args, etc.
    QStringList list;  
    QRegularExpressionMatchIterator m = CommandInterpreter::matchArgsStrict.globalMatch(QRegularExpression::escape(str));
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
        //QColor* bgcol = NULL;

        switch (p.first)
        {
            case NGlobalCmd::CICommand:
            {
                if ( !m_szIconConCommand.isEmpty() && dir.exists(m_szIconConCommand) )
                {
                    iconPath = dir.filePath(m_szIconConCommand);
                }
                
                //if ( m_bHasCmdCol ) bgcol = &m_colBgCommand;
                break;
            }
            
            case NGlobalCmd::CIVariable:
            {
                if ( !m_szIconConVar.isEmpty() && dir.exists(m_szIconConVar) )
                {
                    iconPath = dir.filePath(m_szIconConVar);
                }
                
                //if ( m_bHasVarCol ) bgcol = &m_colBgVariable;
                break;
            }
        }
        
        QListWidgetItem* i = new QListWidgetItem(p.second);
        
        if ( !iconPath.isEmpty() )
        {
            i->setIcon(QIcon(iconPath));
        }
        
//        if ( bgcol )
//        {
//            i->setBackgroundColor(*bgcol);
//        }
        
        m_pSuggestions->addItem(i);
    }
    
    // Sort them alphabetically.
    m_pSuggestions->sortItems();
    
    // Show the suggestions list.
    repositionSuggestions();
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
    
    // Moved elsewhere.
//    // Keep taking away multiples of 120 to find out how many scrolls we need to do.
//    while ( degrees >= 120 )
//    {
//        scrollDown ? moveSuggestionSelectionDown() : moveSuggestionSelectionUp();
//        degrees -= 120;
//    }
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
    if ( suggestionsValid() )
    {
        moveSuggestionSelectionUp();
        replaceWithSuggestion();
    }
}

void CommandEntryBox::chooseBelowSuggestion()
{
    if ( suggestionsValid() )
    {
        moveSuggestionSelectionDown();
        replaceWithSuggestion();
    }
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
