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
    
    // These are not hard-and-fast rules but are guidelines on the way the custom colours are expected to be used.
    // Custom colour 1 - "inactive" text.
    Q_PROPERTY(QColor customColor1 READ customColor1 WRITE setCustomColor1 RESET resetCustomColor1 NOTIFY customColor1Changed)
    
    // Custom colour 2 - highlighted standard text.
    Q_PROPERTY(QColor customColor2 READ customColor2 WRITE setCustomColor2 RESET resetCustomColor2 NOTIFY customColor2Changed)
    
    // Custom colours 3-6 - arbitrary.
    Q_PROPERTY(QColor customColor3 READ customColor3 WRITE setCustomColor3 RESET resetCustomColor3 NOTIFY customColor3Changed)
    Q_PROPERTY(QColor customColor4 READ customColor4 WRITE setCustomColor4 RESET resetCustomColor4 NOTIFY customColor4Changed)
    Q_PROPERTY(QColor customColor5 READ customColor5 WRITE setCustomColor5 RESET resetCustomColor5 NOTIFY customColor5Changed)
    Q_PROPERTY(QColor customColor6 READ customColor6 WRITE setCustomColor6 RESET resetCustomColor6 NOTIFY customColor6Changed)
public:
    explicit ConsoleWidget(QWidget *parent = 0);
    virtual ~ConsoleWidget() {}

    int maxLines() const;
    void setMaxLines(int lines);
    void resetMaxLines();
    
    static const unsigned int DEFAULT_MAX_LINES;
    static const QColor DEFAULT_MESSAGE_COLOUR;
    static const QColor DEFAULT_WARNING_COLOUR;
    static const QColor DEFAULT_CUSTOM_COLOUR1;
    static const QColor DEFAULT_CUSTOM_COLOUR2;
    static const QColor DEFAULT_CUSTOM_COLOUR3;
    static const QColor DEFAULT_CUSTOM_COLOUR4;
    static const QColor DEFAULT_CUSTOM_COLOUR5;
    static const QColor DEFAULT_CUSTOM_COLOUR6;
    
    QColor messageColor() const;
    void setMessageColor(QColor col);
    void resetMessageColor();
    
    QColor warningColor() const;
    void setWarningColor(QColor col);
    void resetWarningColor();
    
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

signals:
    void maxLinesChanged();
    void messageColorChanged();
    void warningColorChanged();
    void customColor1Changed();
    void customColor2Changed();
    void customColor3Changed();
    void customColor4Changed();
    void customColor5Changed();
    void customColor6Changed();

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
    QColor m_colCustom1;
    QColor m_colCustom2;
    QColor m_colCustom3;
    QColor m_colCustom4;
    QColor m_colCustom5;
    QColor m_colCustom6;
};

#endif // CONSOLEWIDGET_H
