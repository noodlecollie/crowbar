#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QTextEdit>
#include <QColor>
#include "interfaces/iconsolewindow.h"

class ICONSOLESHARED_EXPORT ConsoleWindow : public QTextEdit, public IConsoleWindow
{
    Q_OBJECT
    Q_INTERFACES(IConsoleWindow)
public:
    explicit ConsoleWindow(QWidget *parent = 0);

    virtual void printMessage(const QString &message);
    virtual void printWarning(const QString &message);

    void setWarningColour(const QColor col) { m_colWarning = col; }
    QColor getWarningColour() const         { return m_colWarning; }

    void setMessageColour(const QColor col) { m_colStd = col; setTextColor(col); }
    QColor getMessageColour() const         { return m_colStd; }
    
signals:
    
public slots:
    void slotPrintMessage(const QString &message) { printMessage(message); }
    void slotPrintWarning(const QString &message) { printWarning(message); }

private:
    QColor m_colWarning;
    QColor m_colStd;
};

#endif // CONSOLEWINDOW_H
