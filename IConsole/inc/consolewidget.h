#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QTextEdit>
#include "iconsole_global.h"
#include "commandsenderinfo.h"

class ICONSOLESHARED_EXPORT ConsoleWidget : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(int maxLines READ maxLines WRITE setMaxLines RESET resetMaxLines NOTIFY maxLinesChanged)
    Q_PROPERTY(QColor messageColor READ messageColor WRITE setMessageColor RESET resetMessageColor NOTIFY messageColorChanged)
    Q_PROPERTY(QColor warningColor READ warningColor WRITE setWarningColor RESET resetWarningColor NOTIFY warningColorChanged)
public:
    explicit ConsoleWidget(QWidget *parent = 0);
    virtual ~ConsoleWidget() {}

    int maxLines() const;
    void setMaxLines(int lines);
    void resetMaxLines();
    
    static const unsigned int DEFAULT_MAX_LINES;
    static const QColor DEFAULT_MESSAGE_COLOUR;
    static const QColor DEFAULT_WARNING_COLOUR;
    
    QColor messageColor() const;
    void setMessageColor(QColor col);
    void resetMessageColor();
    
    QColor warningColor() const;
    void setWarningColor(QColor col);
    void resetWarningColor();

signals:
    void maxLinesChanged();
    void messageColorChanged();
    void warningColorChanged();

public slots:
    void printMessage(const QString &message);
    void printWarning(const QString &message);
    void printMessage(CommandSenderInfo::OutputType type, const QString &message);
    void repolish();

private:
    void print(const QString &msg);
    void trimToLineLimit();

    int m_iMaxLines;
    QColor m_colMessage;
    QColor m_colWarning;
};

#endif // CONSOLEWIDGET_H
