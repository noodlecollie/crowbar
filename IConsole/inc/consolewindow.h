#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QTextEdit>
#include <QColor>
#include "iconsole_global.h"

class ICONSOLESHARED_EXPORT ConsoleWindow : public QTextEdit
{
    Q_OBJECT
public:
    explicit ConsoleWindow(QWidget *parent = 0);

    void setWarningColour(const QColor &col);
    QColor getWarningColour() const;

    void setMessageColour(const QColor &col);
    QColor getMessageColour() const;
    
signals:
    
public slots:
    void printMessage(const QString &message);
    void printWarning(const QString &message);
private:
    QColor m_colWarning;
    QColor m_colStd;
};

#endif // CONSOLEWINDOW_H
