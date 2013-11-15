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
    Q_PROPERTY(QColor customColor1 READ customColor1 WRITE setCustomColor1 RESET resetCustomColor1 NOTIFY customColor1Changed)
    Q_PROPERTY(QColor customColor2 READ customColor2 WRITE setCustomColor2 RESET resetCustomColor2 NOTIFY customColor2Changed)
    Q_PROPERTY(QColor customColor3 READ customColor3 WRITE setCustomColor3 RESET resetCustomColor3 NOTIFY customColor3Changed)
    Q_PROPERTY(QColor customColor4 READ customColor4 WRITE setCustomColor4 RESET resetCustomColor4 NOTIFY customColor4Changed)
    Q_PROPERTY(QColor customColor5 READ customColor5 WRITE setCustomColor5 RESET resetCustomColor5 NOTIFY customColor5Changed)
    Q_PROPERTY(QColor customColor6 READ customColor6 WRITE setCustomColor6 RESET resetCustomColor6 NOTIFY customColor6Changed)
    Q_PROPERTY(int maxLines READ maxLines WRITE setMaxLines RESET resetMaxLines NOTIFY maxLinesChanged)
public:
    explicit ConsoleWidget(QWidget *parent = 0);
    virtual ~ConsoleWidget() {}
    
    static const QColor DEFAULT_WARNING_COLOUR;
    static const QColor DEFAULT_MESSAGE_COLOUR;
    static const QColor DEFAULT_CUSTOM_COLOUR1;
    static const QColor DEFAULT_CUSTOM_COLOUR2;
    static const QColor DEFAULT_CUSTOM_COLOUR3;
    static const QColor DEFAULT_CUSTOM_COLOUR4;
    static const QColor DEFAULT_CUSTOM_COLOUR5;
    static const QColor DEFAULT_CUSTOM_COLOUR6;
    static const unsigned int DEFAULT_MAX_LINES;
    static const QString ANCHOR_MESSAGE;
    static const QString ANCHOR_WARNING;
    static const QString ANCHOR_CUSTOM1;
    static const QString ANCHOR_CUSTOM2;
    static const QString ANCHOR_CUSTOM3;
    static const QString ANCHOR_CUSTOM4;
    static const QString ANCHOR_CUSTOM5;
    static const QString ANCHOR_CUSTOM6;
    
    QColor warningColor() const;
    void setWarningColor(QColor warningColor);
    void resetWarningColor();
    
    QColor messageColor() const;
    void setMessageColor(QColor messageColor);
    void resetMessageColor();
    
    QColor customColor1() const;
    QColor customColor2() const;
    QColor customColor3() const;
    QColor customColor4() const;
    QColor customColor5() const;
    QColor customColor6() const;
    
    void setCustomColor1(QColor col);
    void setCustomColor2(QColor col);
    void setCustomColor3(QColor col);
    void setCustomColor4(QColor col);
    void setCustomColor5(QColor col);
    void setCustomColor6(QColor col);
    
    void resetCustomColor1();
    void resetCustomColor2();
    void resetCustomColor3();
    void resetCustomColor4();
    void resetCustomColor5();
    void resetCustomColor6();
    
    int maxLines() const;
    void setMaxLines(int lines);
    void resetMaxLines();
    
signals:
    void warningColorChanged();
    void messageColorChanged();
    void customColor1Changed();
    void customColor2Changed();
    void customColor3Changed();
    void customColor4Changed();
    void customColor5Changed();
    void customColor6Changed();
    void maxLinesChanged();
    
public slots:
    void printMessage(const QString &message);
    void printWarning(const QString &message);
    void printMessage(CommandSenderInfo::OutputType type, const QString &message);
    
private:
    void print(const QString &msg, const QString* anchor = NULL);
    void trimToLineLimit();
    void changeBlockColours(const QString &name, const QColor &col);
    //void printCustom(CommandSenderInfo::OutputType type, const QString &msg);
    
    QColor m_colWarning;
    QColor m_colMessage;
    QColor m_colCustom1;
    QColor m_colCustom2;
    QColor m_colCustom3;
    QColor m_colCustom4;
    QColor m_colCustom5;
    QColor m_colCustom6;
    int m_iMaxLines;
};

#endif // CONSOLEWIDGET_H
