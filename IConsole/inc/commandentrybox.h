#ifndef COMMANDENTRYBOX_H
#define COMMANDENTRYBOX_H

#include "iconsole_global.h"
#include <QLineEdit>
#include "commandinterpreter.h"

class CommandSuggestionList;
class QListWidgetItem;

class ICONSOLESHARED_EXPORT CommandEntryBox : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString iconConCommand READ iconConCommand WRITE setIconConCommand RESET resetIconConCommand NOTIFY iconConCommandChanged)
    Q_PROPERTY(QString iconConVar READ iconConVar WRITE setIconConVar RESET resetIconConVar NOTIFY iconConVarChanged)
//    Q_PROPERTY(QColor bgcolorConCommand READ bgcolorConCommand WRITE setBgcolorConCommand RESET resetBgcolorConCommand NOTIFY bgcolorConCommandChanged)
//    Q_PROPERTY(QColor bgcolorConVar READ bgcolorConVar WRITE setBgcolorConVar RESET resetBgcolorConVar NOTIFY bgcolorConVarChanged)
public:
    explicit CommandEntryBox(QWidget *parent = 0);
    virtual ~CommandEntryBox() {}
    
    static const QString LI_NAME_COMMAND;
    static const QString LI_NAME_VARIABLE;
    
    void setSuggestionList(CommandSuggestionList* list);
    CommandSuggestionList* getSuggestionList() const;
    
    QString iconConCommand() const;
    void setIconConCommand(QString icon);
    void resetIconConCommand();
    
    QString iconConVar() const;
    void setIconConVar(QString icon);
    void resetIconConVar();
    
//    QColor bgcolorConCommand() const;
//    void setBgcolorConCommand(QColor col);
//    void resetBgcolorConCommand();
    
//    QColor bgcolorConVar() const;
//    void setBgcolorConVar(QColor col);
//    void resetBgcolorConVar();
    
signals:
    void commandString(const QString&);
    void getSuggestions(const QString&,QList<CommandInterpreter::CommandIdentPair>&, int count = -1);
    void tabPressed();
    void upArrowPressed();
    void downArrowPressed();
    void mouseWheel(int);
    void iconConCommandChanged();
    void iconConVarChanged();
//    void bgcolorConCommandChanged();
//    void bgcolorConVarChanged();
    
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
    
private:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void hideEvent(QHideEvent *e);
    virtual void showEvent(QShowEvent *e);
    virtual void moveEvent(QMoveEvent *e);
    virtual void wheelEvent(QWheelEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    bool insertSuggestion();
    bool suggestionsValid();
    bool replaceWithSuggestion();
    
    CommandSuggestionList*  m_pSuggestions;
    QString                 m_szIconConCommand;
    QString                 m_szIconConVar;
//    QColor                  m_colBgCommand;
//    QColor                  m_colBgVariable;
//    bool                    m_bHasCmdCol;
//    bool                    m_bHasVarCol;
};

#endif // COMMANDENTRYBOX_H
