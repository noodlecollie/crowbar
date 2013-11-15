#ifndef COMMANDENTRYBOX_H
#define COMMANDENTRYBOX_H

#include <QLineEdit>
#include <QList>
#include "iconsole_global.h"
#include "commandinterpreter.h"

class CommandSuggestionList;
class QListWidgetItem;

class ICONSOLESHARED_EXPORT CommandEntryBox : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString iconConCommand READ iconConCommand WRITE setIconConCommand RESET resetIconConCommand NOTIFY iconConCommandChanged)
    Q_PROPERTY(QString iconConVar READ iconConVar WRITE setIconConVar RESET resetIconConVar NOTIFY iconConVarChanged)
    Q_PROPERTY(int commandHistorySize READ commandHistorySize WRITE setCommandHistorySize RESET resetCommandHistorySize NOTIFY commandHistorySizeChanged)
public:
    explicit CommandEntryBox(QWidget *parent = 0);
    virtual ~CommandEntryBox() {}
    
    static const QString LI_NAME_COMMAND;
    static const QString LI_NAME_VARIABLE;
    static const int DEFAULT_COMMAND_HISTORY_SIZE;
    
    void setSuggestionList(CommandSuggestionList* list);
    CommandSuggestionList* getSuggestionList() const;
    
    QString iconConCommand() const;
    void setIconConCommand(QString icon);
    void resetIconConCommand();
    
    QString iconConVar() const;
    void setIconConVar(QString icon);
    void resetIconConVar();
    
    int commandHistorySize() const;
    void setCommandHistorySize(int size);
    void resetCommandHistorySize();
    
signals:
    void commandString(const QString&);
    void getSuggestions(const QString&,QList<CommandInterpreter::CommandIdentPair>&, int count = -1);
    void tabPressed();
    void upArrowPressed();
    void downArrowPressed();
    void mouseWheel(int);
    void iconConCommandChanged();
    void iconConVarChanged();
    void commandHistorySizeChanged();
    
public slots:
    void sendCommandString();
    void repositionSuggestions();
    void moveSuggestionSelectionUp();
    void chooseAboveSuggestion();
    void moveSuggestionSelectionDown();
    void chooseBelowSuggestion();
    void completeWithCurrentSuggestion();
    void scrollSuggestionSelection(int);
    void processForSuggestions(const QString&);
    void itemDoubleClicked(QListWidgetItem* item);
    void clearCommandHistory();
    
private:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void hideEvent(QHideEvent *e);
    virtual void showEvent(QShowEvent *e);
    virtual void moveEvent(QMoveEvent *e);
    virtual void wheelEvent(QWheelEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    bool suggestionsValid();
    bool replaceWithSuggestion(bool shouldRequery = true);
    void traverseHistory(bool direction);
    
    CommandSuggestionList*                  m_pSuggestions;
    QString                                 m_szIconConCommand;
    QString                                 m_szIconConVar;
    bool                                    m_bShouldGetSuggestions;
    QList<QString>                          m_CommandHistory;
    int                                     m_iCommandHistorySize;
    int                                     m_iCurrentHistoryIndex;
};

#endif // COMMANDENTRYBOX_H
