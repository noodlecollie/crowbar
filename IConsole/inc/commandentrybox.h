#ifndef COMMANDENTRYBOX_H
#define COMMANDENTRYBOX_H

#include "iconsole_global.h"
#include <QLineEdit>
#include "commandinterpreter.h"

class CommandSuggestionList;

class ICONSOLESHARED_EXPORT CommandEntryBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit CommandEntryBox(QWidget *parent = 0);
    //explicit CommandEntryBox(CommandInterpreter* interp, QWidget *parent = 0);
    virtual ~CommandEntryBox() {}
    
    //void setInterpreter(CommandInterpreter* interp);
    //CommandInterpreter* getInterpreter() const;
    
    void setSuggestionList(CommandSuggestionList* list);
    CommandSuggestionList* getSuggestionList() const;
    
    static const QString command_img;
    static const QString variable_img;
    
signals:
    void commandString(const QString&);
    void getSuggestions(const QString&,QList<CommandInterpreter::CommandIdentPair>&, int count = -1);
    void tabPressed();
    void upArrowPressed();
    void downArrowPressed();
    void mouseWheel(int);
    
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
};

#endif // COMMANDENTRYBOX_H
