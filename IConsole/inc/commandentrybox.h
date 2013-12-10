/** \file commandentrybox.h
 * \brief Defines the CommandEntryBox class which manages the input of console commands by the user.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef COMMANDENTRYBOX_H
#define COMMANDENTRYBOX_H

#include <QLineEdit>
#include <QList>
#include "iconsole_global.h"
#include "commandinterpreter.h"

class CommandSuggestionList;
class QListWidgetItem;

/**
 * @brief Manages input of console commands by the user.
 *
 * The CommandEntryBox is a specialised QLineEdit which owns a popup suggestion window. Command strings are parsed as the user types
 * them and the getSuggestions signal is emitted whenever command suggestions are required by the entry box. The commandString signal
 * is also emitted when the user's command should be sent to a CommandInterpreter for execution.
 */
class ICONSOLESHARED_EXPORT CommandEntryBox : public QLineEdit
{
    Q_OBJECT
    /** @property iconConCommand
     * @brief Path to icon to display for a ConCommand suggestion entry.
     * @accessors iconConCommand(), setIconConCommand(), resetIconConCommand(), iconConCommandChanged()
     */
    Q_PROPERTY(QString iconConCommand READ iconConCommand WRITE setIconConCommand RESET resetIconConCommand NOTIFY iconConCommandChanged)
    
    /** @property iconConVar
     * @brief Path to icon to display for a ConVar suggestion entry.
     * @accessors iconConVar(), setIconConVar(), resetIconConVar(), iconConVarChanged()
     */
    Q_PROPERTY(QString iconConVar READ iconConVar WRITE setIconConVar RESET resetIconConVar NOTIFY iconConVarChanged)
    
    /** @property commandHistorySize
     * @brief Size of the command history buffer.
     * @accessors commandHistorySize(), setCommandHistorySize(), resetCommandHistorySize(), commandHistorySizeChanged()
     */
    Q_PROPERTY(int commandHistorySize READ commandHistorySize WRITE setCommandHistorySize RESET resetCommandHistorySize NOTIFY commandHistorySizeChanged)
public:
    /**
     * @brief Constructor.
     * @param parent QWidget parent, if applicable.
     */
    explicit CommandEntryBox(QWidget *parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~CommandEntryBox() {}
    
    /**
     * @brief Identifying name of a command suggestion list item which contains the name of a console command.
     */
    static const QString LI_NAME_COMMAND;
    
    /**
     * @brief Identifying name of a command suggestion list item which contains the name of a console variable.
     */
    static const QString LI_NAME_VARIABLE;
    
    /**
     * @brief Default size of the command history buffer (how many previously typed commands this entry
     * box will remember.
     */
    static const int DEFAULT_COMMAND_HISTORY_SIZE;
    
//    void setSuggestionList(CommandSuggestionList* list);
    
    /**
     * @brief Returns the command suggestion list belonging to this entry box.
     * @return Suggestion list for this entry box.
     */
    const CommandSuggestionList* getSuggestionList() const;
    
    /**
     * @brief Returns the file name and path of the icon displayed in the suggestion box
     * for a console command entry. Path is relative to the application directory.
     * @return Path to the current icon displayed, or an empty string if no icon should be displayed.
     */
    QString iconConCommand() const;
    
    /**
     * @brief Sets the file name and path of the icon to display in the suggestion box
     * next to a console command entry. Path should be relative to the application directory.
     * @param icon Path to icon file, including extension.
     */
    void setIconConCommand(QString icon);
    
    /**
     * @brief Resets (blanks) the icon path string for the console command list entry icon.
     */
    void resetIconConCommand();
    
    /**
     * @brief Returns the file name and path of the icon displayed in the suggestion box
     * for a console variable entry. Path is relative to the application directory.
     * @return Path to the current icon displayed, or an empty string if no icon should be displayed.
     */
    QString iconConVar() const;
    
    /**
     * @brief Sets the file name and path of the icon to display in the suggestion box
     * next to a console variable entry. Path should be relative to the application directory.
     * @param icon Path to icon file, including extension.
     */
    void setIconConVar(QString icon);
    
    /**
     * @brief Resets (blanks) the icon path string for the console command list entry icon.
     */
    void resetIconConVar();
    
    /**
     * @brief Returns the current size of the command history buffer - how many previously typed
     * commands the entry box will remember.
     * @return Size of command history buffer.
     */
    int commandHistorySize() const;
    
    /**
     * @brief Sets the size of the command history buffer.
     * @note Older commands will be removed from the history if they fall outside the new buffer size.
     * @param size Size to set.
     */
    void setCommandHistorySize(int size);
    
    /**
     * @brief Resets the command history buffer to the default size.
     * @note Older commands will be removed from the history if they fall outside the new buffer size.
     */
    void resetCommandHistorySize();
    
signals:
    /**
     * @brief Emitted when the user executes the command string they have been entering.
     * @param command Command string the user entered.
     */
    void commandString(const QString &command);
    
    /**
     * @brief Emitted when the entry box requests that its command suggestion list be filled with suggestions for the
     * current command string prefix.
     * @param prefix The string the user has entered so far, to be matched against the beginning of pre-existing commands.
     * @param list List to fill with suggestion entries.
     * @param count Maximum number of suggestions desired, or -1 if no limit.
     */
    void getSuggestions(const QString &prefix, QList<CommandInterpreter::CommandIdentPair> &list, int count = -1);
    
    /**
     * @brief Tab has been pressed while the entry box has focus.
     */
    void tabPressed();
    
    /**
     * @brief Up arrow has been pressed while the entry box has focus.
     */
    void upArrowPressed();
    
    /**
     * @brief Down arrow has been pressed while the entry box has focus.
     */
    void downArrowPressed();
    
    /**
     * @brief The user moved the mouse wheel while the entry box has focus.
     * @param deltaDegrees Number of degrees the mouse wheel was moved.
     */
    void mouseWheel(int deltaDegrees);
    
    /**
     * @brief The console command icon path has been changed.
     */
    void iconConCommandChanged();
    
    /**
     * @brief The console variable icon path has been changed.
     */
    void iconConVarChanged();
    
    /**
     * @brief The command history buffer size has been changed.
     */
    void commandHistorySizeChanged();
    
public slots:
    /**
     * @brief Sends the current command string. Results in the commandString signal being emitted.
     */
    void sendCommandString();
    
    /**
     * @brief Tells the entry box that the suggestion list should be repositioned to the entry box's
     * current position.
     */
    void repositionSuggestions();
    
    /**
     * @brief Moves the current selection in the command suggestion box up one entry.
     */
    void moveSuggestionSelectionUp();
    
    /**
     * @brief If the suggestion box is present, chooses the entry directly above the current suggestion and
     * inserts the text into the entry box. The suggestion list is not closed.
     * If the suggestion box is not present, moves back one in the command history and inserts the command string
     * into the entry box.
     */
    void chooseAboveSuggestion();
    
    /**
     * @brief Moves the current selection in the command suggestion box down one entry.
     */
    void moveSuggestionSelectionDown();
    
    /**
     * @brief If the suggestion box is present, chooses the entry directly below the current suggestion and
     * inserts the text into the entry box. The suggestion list is not closed.
     * If the suggestion box is not present, moves forward one in the command history and inserts the command string
     * into the entry box.
     */
    void chooseBelowSuggestion();
    
    /**
     * @brief Chooses the currently selected command suggestion and insert it into the entry box before closing the
     * suggestion list.
     */
    void completeWithCurrentSuggestion();
    
    /**
     * @brief Scrolls \a n entries up or down in the suggestion list and inserts the resulting selection into the
     * entry box.
     * @param n Number of entries to scroll. Positive values scroll up, negative values scroll down.
     */
    void scrollSuggestionSelection(int n);
    
    /**
     * @brief Processes the current partial command string and requests suggestions if required.
     * @param str Command string, possibly incomplete, to process.
     */
    void processForSuggestions(const QString &str);
    
    /**
     * @brief Inserts the command held by the list item into the entry box.
     * @param item Item holding the command to insert.
     */
    void completeListItem(QListWidgetItem* item);
    
    /**
     * @brief Clears the command history buffer.
     */
    void clearCommandHistory();
    
private:
    /**
     * @brief Handler for the key press event.
     * @param e Event details.
     */
    virtual void keyPressEvent(QKeyEvent *e);
    
    /**
     * @brief Handler for the hide event.
     * @param e Event details.
     */
    virtual void hideEvent(QHideEvent *e);
    
    /**
     * @brief Handler for the show event.
     * @param e Event details.
     */
    virtual void showEvent(QShowEvent *e);
    
    /**
     * @brief Handler for the move event.
     * @param e Event details.
     */
    virtual void moveEvent(QMoveEvent *e);
    
    /**
     * @brief Handler for the mouse wheel event.
     * @param e Event details.
     */
    virtual void wheelEvent(QWheelEvent *e);
    
    /**
     * @brief Handler for the focus out event.
     * @param e Event details.
     */
    virtual void focusOutEvent(QFocusEvent *e);
    
    /**
     * @brief Returns whether the suggestion box is currently valid (exists, is visible, contains suggestions).
     * @return True if the suggestion box is valid, false otherwise.
     */
    bool suggestionsValid();
    
    /**
     * @brief Replaces the current entry box text with the command
     * @param shouldRequery If true, suggestion box is re-queried with new command string.
     * @return True on success, false if the suggestion to replace with was not valid.
     */
    bool replaceWithSuggestion(bool shouldRequery = true);
    
    /**
     * @brief Moves forwards or backwards in the command history by one command.
     * @param direction True moves backwards in the history, false moves forwards.
     */
    void traverseHistory(bool direction);
    
    /**
     * @brief Inserts the rest of the suggestion into the entry box without replacing the rest of the text.
     * @param shouldRequery If true, suggestion box is re-queried with new command string.
     * @return True on success, false if the suggestion to insert is not valid.
     */
    bool insertSuggestion(bool shouldRequery = true);
    
    CommandSuggestionList*                  m_pSuggestions;             /**< Pointer to suggestion list */
    QString                                 m_szIconConCommand;         /**< ConCommand icon path */
    QString                                 m_szIconConVar;             /**< ConVar icon command */
    bool                                    m_bShouldGetSuggestions;    /**< Suggestions only queried if this is true */
    QStringList                             m_CommandHistory;           /**< String list of previous commands */
    int                                     m_iCommandHistorySize;      /**< Maximum size of previous commands kept in history */
    int                                     m_iCurrentHistoryIndex;     /**< Index of the current command we are looking at in history */
};

#endif // COMMANDENTRYBOX_H

/**@}*/
