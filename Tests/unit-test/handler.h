#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>

class Handler : public QObject
{
    Q_OBJECT
public:
    explicit Handler(QObject *parent = 0);
    
signals:
    void out();
    
public slots:
    void in() { emit out(); }
    
};

#endif // HANDLER_H
