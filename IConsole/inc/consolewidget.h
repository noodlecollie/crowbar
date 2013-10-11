#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QTextEdit>
#include <QColor>
#include "iconsole_global.h"
#include "commandsenderinfo.h"

class ICONSOLESHARED_EXPORT ConsoleWidget : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor warningColor READ warningColor WRITE setWarningColor RESET resetWarningColor NOTIFY warningColorChanged)
    Q_PROPERTY(QColor messageColor READ messageColor WRITE setMessageColor RESET resetMessageColor NOTIFY messageColorChanged)
public:
    explicit ConsoleWidget(QWidget *parent = 0);
    virtual ~ConsoleWidget() {}
    
    static const QColor DEFAULT_WARNING_COLOUR;
    static const QColor DEFAULT_MESSAGE_COLOUR;
    
    QColor warningColor() const;
    void setWarningColor(QColor warningColor);
    void resetWarningColor();
    
    QColor messageColor() const;
    void setMessageColor(QColor messageColor);
    void resetMessageColor();
    
signals:
    void warningColorChanged();
    void messageColorChanged();
    
public slots:
    void printMessage(const QString &message);
    void printWarning(const QString &message);
    void printMessage(CommandSenderInfo::OutputType type, const QString &message);
    
private:
    void print(const QString &msg);
    QColor m_colWarning;
    QColor m_colMessage;
};

#endif // CONSOLEWIDGET_H
