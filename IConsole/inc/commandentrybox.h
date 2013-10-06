#ifndef COMMANDENTRYBOX_H
#define COMMANDENTRYBOX_H

#include "iconsole_global.h"
#include <QLineEdit>

class CommandInterpreter;
class CommandSuggestionList;

class ICONSOLESHARED_EXPORT CommandEntryBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit CommandEntryBox(QWidget *parent = 0);
    explicit CommandEntryBox(CommandInterpreter* interp, QWidget *parent = 0);
    virtual ~CommandEntryBox() {}
    
    void setInterpreter(CommandInterpreter* interp);
    CommandInterpreter* getInterpreter() const;
    
    void setSuggestionList(CommandSuggestionList* list);
    CommandSuggestionList* getSuggestionList() const;
    
signals:
    
public slots:
    void sendCommandString();
    void repositionSuggestions();
    
private:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void hideEvent(QHideEvent *e);
    virtual void showEvent(QShowEvent *e);
    virtual void moveEvent(QMoveEvent *e);
    virtual void wheelEvent(QWheelEvent *e);
    bool insertSuggestion();
    bool suggestionsValid();
    bool replaceWithSuggestion();
    
    CommandInterpreter*     m_pInterpreter;
    CommandSuggestionList*  m_pSuggestions;
};

#endif // COMMANDENTRYBOX_H
