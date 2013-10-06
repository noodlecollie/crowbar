#ifndef GLOBALOUTPUTREDIRECTOR_H
#define GLOBALOUTPUTREDIRECTOR_H

#include <QObject>
#include "globals.h"
#include "consolewidget.h"

// Prints any text passed to redirect() using LogMessage.
class GlobalOutputRedirector : public QObject
{
    Q_OBJECT
public:
    explicit GlobalOutputRedirector(QObject* parent = 0) : QObject(parent) {}
    virtual ~GlobalOutputRedirector() {}
    
public slots:
    void redirectMessage(QString &message)
    {
        LogMessage(message);
    }
    
    void redirectWarning(QString &warning)
    {
        LogWarning(warning);
    }
};

#endif // GLOBALOUTPUTREDIRECTOR_H
