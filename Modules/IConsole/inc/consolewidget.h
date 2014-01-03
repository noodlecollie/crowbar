/** \file consolewidget.h
 * \brief Defines the ConsoleWidget class which displays console output.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QTextEdit>
#include "iconsole_global.h"
#include "commandsenderinfo.h"

ICONSOLE_BEGIN_NAMESPACE

/**
 * @brief Displays output from the console system. Is usually grouped with a CommandEntryBox to form a full console window.
 *
 * The ConsoleWidget supports 8 different message colours - standard message, warning and custom 1-6. Custom colours
 * 1 and 2 are usually used for inactive and highlighted (ie. important) text respectively. These colours can be set via
 * the "qproperty-<name>" syntax in the application style sheet.
 */
class ICONSOLESHARED_EXPORT ConsoleWidget : public QTextEdit
{
    Q_OBJECT
    
    /** @property maxLines
     * @brief Maximum number of lines that are kept visible (ie. scrollable) in the window.
     * @accessors maxLines() setMaxLines() resetMaxLines() maxLinesChanged()
     */
    Q_PROPERTY(int maxLines READ maxLines WRITE setMaxLines RESET resetMaxLines NOTIFY maxLinesChanged)
    
    /** @property messageColor
     * @brief Colour of standard messages printed to the window.
     * @accessors messageColor() setMessageColor() resetMessageColor() messageColorChanged()
     */
    Q_PROPERTY(QColor messageColor READ messageColor WRITE setMessageColor RESET resetMessageColor NOTIFY messageColorChanged)
    
    /** @property warningColor
     * @brief Colour of warning messages printed to the window.
     * @accessors warningColor() setWarningColor() resetWarningColor() warningColorChanged()
     */
    Q_PROPERTY(QColor warningColor READ warningColor WRITE setWarningColor RESET resetWarningColor NOTIFY warningColorChanged)
    
    // These are not hard-and-fast rules but are guidelines on the way the custom colours are expected to be used.
    /** @property customColor1
     * @brief Custom message colour 1 - intended for inactive (faded) text.
     * @accessors customColor1() setCustomColor1() resetCustomColor1() customColor1Changed()
     */
    Q_PROPERTY(QColor customColor1 READ customColor1 WRITE setCustomColor1 RESET resetCustomColor1 NOTIFY customColor1Changed)
    
    /** @property customColor2
     * @brief Custom message colour 2 - intended for highlighted (important) text.
     * @accessors customColor2() setCustomColor2() resetCustomColor2() customColor2Changed()
     */
    Q_PROPERTY(QColor customColor2 READ customColor2 WRITE setCustomColor2 RESET resetCustomColor2 NOTIFY customColor2Changed)
    
    // Custom colours 3-6 - arbitrary.
    /** @property customColor3
     * @brief Custom message colour 3.
     * @accessors customColor3() setCustomColor3() resetCustomColor3() customColor3Changed()
     */
    Q_PROPERTY(QColor customColor3 READ customColor3 WRITE setCustomColor3 RESET resetCustomColor3 NOTIFY customColor3Changed)
    
    /** @property customColor4
     * @brief Custom message colour 4.
     * @accessors customColor4() setCustomColor4() resetCustomColor4() customColor4Changed()
     */
    Q_PROPERTY(QColor customColor4 READ customColor4 WRITE setCustomColor4 RESET resetCustomColor4 NOTIFY customColor4Changed)
    
    /** @property customColor5
     * @brief Custom message colour 5.
     * @accessors customColor5() setCustomColor5() resetCustomColor5() customColor5Changed()
     */
    Q_PROPERTY(QColor customColor5 READ customColor5 WRITE setCustomColor5 RESET resetCustomColor5 NOTIFY customColor5Changed)
    
    /** @property customColor6
     * @brief Custom message colour 6.
     * @accessors customColor6() setCustomColor6() resetCustomColor6() customColor6Changed()
     */
    Q_PROPERTY(QColor customColor6 READ customColor6 WRITE setCustomColor6 RESET resetCustomColor6 NOTIFY customColor6Changed)
public:
    /**
     * @brief Constructor.
     * @param parent Parent QWidget, if applicable.
     */
    explicit ConsoleWidget(QWidget *parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~ConsoleWidget() {}
    
public:
    /**
     * @brief Default maximum line count of the window.
     */
    static const unsigned int DEFAULT_MAX_LINES;
    
    /**
     * @brief Default message colour.
     */
    static const QColor DEFAULT_MESSAGE_COLOUR;
    
    /**
     * @brief Default warning colour.
     */
    static const QColor DEFAULT_WARNING_COLOUR;
    
    /**
     * @brief Default colour for custom1.
     */
    static const QColor DEFAULT_CUSTOM_COLOUR1;
    
    /**
     * @brief Default colour for custom2.
     */
    static const QColor DEFAULT_CUSTOM_COLOUR2;
    
    /**
     * @brief Default colour for custom3.
     */
    static const QColor DEFAULT_CUSTOM_COLOUR3;
    
    /**
     * @brief Default colour for custom4.
     */
    static const QColor DEFAULT_CUSTOM_COLOUR4;
    
    /**
     * @brief Default colour for custom5.
     */
    static const QColor DEFAULT_CUSTOM_COLOUR5;
    
    /**
     * @brief Default colour for custom6.
     */
    static const QColor DEFAULT_CUSTOM_COLOUR6;
    
public:
    /**
     * @brief Gets the maximum number of lines of output kept in the window's history.
     * @return Max lines for the window.
     */
    int maxLines() const;
    
    /**
     * @brief Sets the maximum number of lines of output kept in the window's history.
     * @param lines Max lines of output this window should keep.
     */
    void setMaxLines(int lines);
    
    /**
     * @brief Resets the max line count to its default value.
     */
    void resetMaxLines();
    
    /**
     * @brief Gets the current standard message colour.
     * @return Standard message colour.
     */
    QColor messageColor() const;
    
    /**
     * @brief Sets the standard message colour.
     * @param col Colour to set.
     */
    void setMessageColor(QColor col);
    
    /**
     * @brief Resets the standard message colour to the default.
     */
    void resetMessageColor();
    
    /**
     * @brief Gets the current warning message colour.
     * @return Warning message colour.
     */
    QColor warningColor() const;
    
    /**
     * @brief Sets the warning message colour.
     * @param col Colour to set.
     */
    void setWarningColor(QColor col);
    
    /**
     * @brief Resets the warning message colour to the default.
     */
    void resetWarningColor();
    
    /**
     * @brief Gets the first custom message colour.
     * @return Custom colour 1.
     */
    QColor customColor1() const;
    
    /**
     * @brief Sets the first custom message colour.
     * @param col Colour to set.
     */
    void setCustomColor1(QColor col);
    
    /**
     * @brief Resets the first custom message colour to its default.
     */
    void resetCustomColor1();
    
    /**
     * @brief Gets the second custom message colour.
     * @return Custom colour 2.
     */
    QColor customColor2() const;
    
    /**
     * @brief Sets the second custom message colour.
     * @param col Colour to set.
     */
    void setCustomColor2(QColor col);
    
    /**
     * @brief Resets the second custom message colour to its default.
     */
    void resetCustomColor2();
    
    /**
     * @brief Gets the third custom message colour.
     * @return Custom colour 3.
     */
    QColor customColor3() const;
    
    /**
     * @brief Sets the third custom message colour.
     * @param col Colour to set.
     */
    void setCustomColor3(QColor col);
    
    /**
     * @brief Resets the third custom message colour to its default.
     */
    void resetCustomColor3();
    
    /**
     * @brief Gets the fourth custom message colour.
     * @return Custom colour 4.
     */
    QColor customColor4() const;
    
    /**
     * @brief Sets the fourth custom message colour.
     * @param col Colour to set.
     */
    void setCustomColor4(QColor col);
    
    /**
     * @brief Resets the fourth custom message colour to its default.
     */
    void resetCustomColor4();
    
    /**
     * @brief Gets the fifth custom message colour.
     * @return Custom colour 5.
     */
    QColor customColor5() const;
    
    /**
     * @brief Sets the fifth custom message colour.
     * @param col Colour to set.
     */
    void setCustomColor5(QColor col);
    
    /**
     * @brief Resets the fifth custom message colour to its default.
     */
    void resetCustomColor5();
    
    /**
     * @brief Gets the sixth custom message colour.
     * @return Custom colour 6.
     */
    QColor customColor6() const;
    
    /**
     * @brief Sets the sixth custom message colour.
     * @param col Colour to set.
     */
    void setCustomColor6(QColor col);
    
    /**
     * @brief Resets the sixth custom message colour to its default.
     */
    void resetCustomColor6();
    
signals:
    
    /**
     * @brief Emitted when the max lines property has been changed.
     */
    void maxLinesChanged(int);
    
    /**
     * @brief Emitted when the standard message colour property has been changed.
     */
    void messageColorChanged(QColor);
    
    /**
     * @brief Emitted when the warning message colour property has been changed.
     */
    void warningColorChanged(QColor);
    
    /**
     * @brief Emitted when the first custom message colour property has been changed.
     */
    void customColor1Changed(QColor);
    
    /**
     * @brief Emitted when the second custom message colour property has been changed.
     */
    void customColor2Changed(QColor);
    
    /**
     * @brief Emitted when the third custom message colour property has been changed.
     */
    void customColor3Changed(QColor);
    
    /**
     * @brief Emitted when the fourth custom message colour property has been changed.
     */
    void customColor4Changed(QColor);
    
    /**
     * @brief Emitted when the fifth custom message colour property has been changed.
     */
    void customColor5Changed(QColor);
    
    /**
     * @brief Emitted when the sixth custom message colour property has been changed.
     */
    void customColor6Changed(QColor);

public slots:
    
    /**
     * @brief Prints a standard message to the window.
     * @param message Message to print.
     */
    void printMessage(const QString &message);
    
    /**
     * @brief Prints a warning message to the window.
     * @param message Message to print.
     */
    void printWarning(const QString &message);
    
    /**
     * @brief Prints a message to the window.
     * @param type Type of message to print.
     * @param message Message text.
     */
    void printMessage(CommandSenderInfo::OutputType type, const QString &message);
    
    /**
     * @brief Refreshes the style on the widget. Ensures the values of any QProperties set via stylesheet are current.
     */
    void repolish();

private:
    
    /**
     * @brief Inserts text at the end of the current content.
     * @param msg Text to insert.
     */
    void print(const QString &msg);
    
    /**
     * @brief Removes any lines that exceed the current max line count from the top of the window.
     */
    void trimToLineLimit();

private:
    int m_iMaxLines;        /**< Max number of output lines stored by the window. */
    QColor m_colMessage;    /**< Standard message colour. */
    QColor m_colWarning;    /**< Warning message colour. */
    QColor m_colCustom1;    /**< Custom message colour 1. */
    QColor m_colCustom2;    /**< Custom message colour 2. */
    QColor m_colCustom3;    /**< Custom message colour 3. */
    QColor m_colCustom4;    /**< Custom message colour 4. */
    QColor m_colCustom5;    /**< Custom message colour 5. */
    QColor m_colCustom6;    /**< Custom message colour 6. */
};

ICONSOLE_END_NAMESPACE
    
#endif // CONSOLEWIDGET_H

/**@}*/
