/** \file convar.h
 * \brief Defines the ConVar class for storing values.
 */

/**
 * \defgroup IConsole IConsole library
 * @{
 */

#ifndef CONVAR_H
#define CONVAR_H

#include "iconsole_global.h"
#include <QString>
#include <QVariant>
#include "nglobalcmd.h"
#include "listedconsolecommand.h"

class CommandManager;
class CommandSenderInfo;

/**
 * @brief Stores values that are retrievable and settable by the user (from the console) or by the application (via code).
 *
 * ConVars work almost exactly the same way as in the Source engine: specifying a variable name in the console followed by a value
 * will set the variable to that value, and just specifying the name on its own will print the current value of the variable to the
 * console. Variables can also contain optional callback pointers which are run just before the variable's value is changed; the
 * proposed input value can be modified/validated by the callback before it is physically set. Note that the callback is also called
 * on setToDefault().<br/><br/>
 * Values are stored physically in the ConVar as QVariants but are set at a raw level (and passed to/from the validation callbacks)
 * as strings. There are also convenience functions to get or set the value of a ConVar as a bool, int or float - these will return
 * false or 0 respectively if the value cannot be converted.
 */
class ICONSOLESHARED_EXPORT ConVar : public ListedConsoleCommand
{
    Q_OBJECT
    
    Q_PROPERTY(QString stringValue READ stringValue WRITE setValue)
    Q_PROPERTY(bool boolValue READ boolValue WRITE setValue)
    Q_PROPERTY(int intValue READ intValue WRITE setValue)
    Q_PROPERTY(float floatValue READ floatValue WRITE setValue)
public:
    /**
     * @brief Constructor.
     * @note If min/max values are set, attempting to set a string value which cannot be cast to a numerical
     * value will fail.
     * @param name Name of the variable.
     * @param def Default value of the variable.
     * @param callback Optional callback pointer to be run just before the variable's value is changed.
     * @param desc Optional description of the variable.
     * @param flags Variable flags.
     * @param hasMin True if the variable should have a minimum value. Numerical values outside this range are clamped.
     * @param min 
     * @param hasMax
     * @param max
     * @param parent
     */
    explicit ConVar(const QString &name, const QString &def, NGlobalCmd::VarCallback callback = NULL, const QString &desc = "",
                    NGlobalCmd::CMDFLAGS flags = 0, bool hasMin = false, float min = 0.0, bool hasMax = false, float max = 0.0, QObject *parent = 0);
    
    explicit ConVar(const QString &name, const QString &def, CommandManager* manager, ListedConsoleCommand** list,
                    NGlobalCmd::VarCallback callback = NULL, const QString &desc = "", NGlobalCmd::CMDFLAGS flags = 0,
                    bool hasMin = false, float min = 0.0f, bool hasMax = false, float max = 0.0f, QObject* parent = 0);
    
    virtual ~ConVar() {}
    
    virtual NGlobalCmd::CmdIdent identify() const;
    void setCallback(NGlobalCmd::VarCallback callback);
    NGlobalCmd::VarCallback getCallback() const;
    QString get() const;
    QString set(const QString &value);
    QString set(const CommandSenderInfo &info, const QString &value);
    bool hasMin() const;
    float getMin() const;
    void setMin(float value);
    bool hasMax() const;
    float getMax() const;
    void setMax(float value);
    QString getDefault() const;
    void setToDefault();
    
    QString stringValue() const;
    QString setValue(const QString &val);
    QString setValue(const char* val);
    int intValue() const;
    int setValue(int val);
    float floatValue() const;
    float setValue(float val);
    bool boolValue() const;
    bool setValue(bool val);
    
private:
    void validateBounds(float &min, float &max);
    float clamp(float value);
    int clamp(int value);
    
    NGlobalCmd::VarCallback     m_pVarCallback;
    QVariant                    m_Variable;
    QVariant                    m_Default;
    bool                        m_bHasMin;
    bool                        m_bHasMax;
    float                       m_flMinVal;
    float                       m_flMaxVal;
};

#endif // CONVAR_H

/**@}*/
