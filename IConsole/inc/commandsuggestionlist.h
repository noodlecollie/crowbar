#ifndef COMMANDSUGGESTIONLIST_H
#define COMMANDSUGGESTIONLIST_H

#include "iconsole_global.h"
#include <QListWidget>

class QString;

class ICONSOLESHARED_EXPORT CommandSuggestionList : public QListWidget
{
    Q_OBJECT
public:
    explicit CommandSuggestionList(QWidget *parent = 0);
    virtual ~CommandSuggestionList() {}
    
    QString getCurrentSelection();
    bool hasSelection() const;
    
signals:
    
public slots:
    void moveUp();
    void moveDown();
    void selectFirst();
    
private:
};

#endif // COMMANDSUGGESTIONLIST_H
