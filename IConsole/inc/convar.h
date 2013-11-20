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
 * false or 0 respectively if the value cannot be converted.<br/><br/>
 * If min or max values are set on a ConVar, set values will be clamped between the respective min or max. Integers will always be clamped
 * to the nearest whole number either above the min or below the max; if this is not possible (ie. the bounds are too close together),
 * the operation will fail. Boolean values are set as integers (and so also conform to this rule) - a value of zero is false and any other
 * value represents true. If a string value is set when a variable has a min or max value, the operation will fail if the string cannot be
 * converted to a float value; if it can, the float clamping rules will apply. 
 */
class ICONSOLESHARED_EXPORT ConVar : public ListedConsoleCommand
{
    Q_OBJECT
    
    /** @property stringValue
     * @brief Variable's value as a string.
     * @accessors stringValue(), setValue()
     */
    Q_PROPERTY(QString stringValue READ stringValue WRITE setValue)
    
    /** @property boolValue
     * @brief Variable's value as a boolean.
     * @accessors boolValue(), setValue()
     */
    Q_PROPERTY(bool boolValue READ boolValue WRITE setValue)
    
    /** @property intValue
     * @brief Variable's value as an int.
     * @accessors intValue(), setValue()
     */
    Q_PROPERTY(int intValue READ intValue WRITE setValue)
    
    /** @property floatValue
     * @brief Variable's value as a float.
     * @accessors floatValue(), setValue()
     */
    Q_PROPERTY(float floatValue READ floatValue WRITE setValue)
public:
    /**
     * @brief Constructor.
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
    
    /**
     * @brief Constructor.
     * @param name Name of the variable.
     * @param def Default value of the variable.
     * @param manager Manager to attempt to register to when this variable is constructed.
     * @param list If the variable could not register with the manager, it attaches itself to this list instead.
     * @param callback Optional callback pointer to be run just before the variable's value is changed.
     * @param desc Optional description of the variable.
     * @param flags Variable flags.
     * @param hasMin True if the variable should have a minimum value. Numerical values outside this range are clamped.
     * @param min 
     * @param hasMax
     * @param max
     * @param parent
     */
    explicit ConVar(const QString &name, const QString &def, CommandManager* manager, ListedConsoleCommand** list,
                    NGlobalCmd::VarCallback callback = NULL, const QString &desc = "", NGlobalCmd::CMDFLAGS flags = 0,
                    bool hasMin = false, float min = 0.0f, bool hasMax = false, float max = 0.0f, QObject* parent = 0);
    
    /**
     * @brief Destructor.
     */
    virtual ~ConVar() {}
    
    /**
     * @brief Returns the identifier for this command.
     * @return CmdIdent::CIVariable for console variables.
     */
    virtual NGlobalCmd::CmdIdent identify() const;
    
    /**
     * @brief Sets the variable change callback.
     * @param callback Callback pointer to set.
     */
    void setCallback(NGlobalCmd::VarCallback callback);
    
    /**
     * @brief Gets the current variable change callback
     * @return Pointer to current callback, or NULL if none exists.
     */
    NGlobalCmd::VarCallback getCallback() const;
    
    /**
     * @brief Returns whether or not this variable has a min value.
     * @return True if the variable has a min value, false otherwise.
     */
    bool hasMin() const;
    
    /**
     * @brief The minimum value of this variable.
     * @note Integer/boolean values will always be clamped to values above this.
     * @warning If hasMin() is false, the value this function returns is undefined.
     * @return Minimum value of the variable.
     */
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
    
    virtual void setFlagsRaw(NGlobalCmd::CMDFLAGS flags);
    virtual void setFlag(NGlobalCmd::CMDFLAGS flag);
    virtual void toggleFlag(NGlobalCmd::CMDFLAGS flag);
    
private:
    void validateBounds(float &min, float &max);
    float clamp(float value);
    int clamp(int value);
    bool canSetInt();
    QString get() const;
    QString set(const QString &value);
    QString set(const CommandSenderInfo &info, const QString &value);
    
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
