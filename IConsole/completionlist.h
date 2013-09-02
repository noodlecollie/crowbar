#ifndef COMPLETIONLIST_H
#define COMPLETIONLIST_H

#include <QListWidget>
#include "iconsole_global.h"

class QKeyEvent;

// Adapted from QConsole code at http://qconsole.sourceforge.net/

class ICONSOLESHARED_EXPORT CompletionList : public QListWidget
{
    Q_OBJECT
public:
    explicit CompletionList(QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:
    virtual QSize sizeHint() const;
    virtual void keyPressEvent(QKeyEvent *e);
};

#endif // COMPLETIONLIST_H
