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
 * Variable clamping is performed in two ways - for floats, the value is set to be either the min or max value if it exceeds these bounds.
 * Integer values are instead set to the nearest integer inside the relevant bound.
 * Setting a min or max value on a ConVar will result in the stored value being immediately clamped by the float clamping rules if it is
 * outside the new bound. If min or max values exist when setting a value, the value will also immediately be clamped between the respective
 * min or max. If it is not possible to clamp when setting an integer (ie. the bounds are too close together), the operation will fail.
 * Boolean values are set as integers (and so also conform to this rule) - a value of zero is false and any other value represents true.
 * If a string value is set when a variable has a min or max value, the operation will fail if the string cannot be converted to a float
 * value; if it can, the float clamping rules will apply. 
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
    
    /** @property hasMin
     * @brief Whether the variable has a minimum value.
     * @accessors hasMin(), setHasMin()
     */
    Q_PROPERTY(bool hasMin READ hasMin WRITE setHasMin)
    
    /** @property hasMax
     * @brief Whether the variable has a maximum value.
     * @accessors hasMax(), setHasMax()
     */
    Q_PROPERTY(bool hasMax READ hasMax WRITE setHasMax)
    
    /** @property maxValue
     * @brief Maximum value allowed for the variable. Only valid if hasMax() is true.
     * @accessors maxValue(), setMaxValue()
     */
    Q_PROPERTY(float maxValue READ maxValue WRITE setMaxValue)
    
    /** @property minValue
     * @brief Minimum value allowed for the variable. Only valid if hasMin() is true.
     * @accessors minValue(), setMinValue()
     */
    Q_PROPERTY(float minValue READ minValue WRITE setMinValue)
    
    /** @property defaultValue
     * @brief Default value for the variable.
     * @accessors defaultValue()
     */
    Q_PROPERTY(QString defaultValue READ defaultValue)
    
    /** @property canSetInt
     * @brief Whether the bounds of the variable have enough space between them for integer values to be set.
     * @accessors canSetInt()
     */
    Q_PROPERTY(bool canSetInt READ canSetInt)
public:
    /**
     * @brief Constructor.
     * @param name Name of the variable.
     * @param def Default value of the variable.
     * @param callback Optional callback pointer to be run just before the variable's value is changed.
     * @param desc Optional description of the variable.
     * @param flags Variable flags.
     * @param hasMin True if the variable should have a minimum value. Numerical values outside this range are clamped.
     * @param min Minimum value for the variable.
     * @param hasMax True if the variable should have a maximum value. Numerical values outside this range are clamped.
     * @param max Maximum value for the variable.
     * @param parent Parent QObject, if applicable.
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
     * @param min Minimum value for the variable.
     * @param hasMax True if the variable should have a maximum value. Numerical values outside this range are clamped.
     * @param max Maximum value for the variable.
     * @param parent Parent QObject, if applicable.
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
     * @brief Sets whether this variable has a minimum value.
     * @note If true is passed and the current value is less than the min, it will
     * be clamped according to the float clamping rules.
     * @param b True if variable should have minimum value, false otherwise.
     */
    void setHasMin(bool b);
    
    /**
     * @brief Sets whether this variable has a minimum value.
     * @note If true is passed and the current value is less than the min, it will
     * be clamped according to the float clamping rules.
     * @param info CommandSenderInfo for printing messages.
     * @param b True if variable should have minimum value, false otherwise.
     */
    void setHasMin(CommandSenderInfo &info, bool b);
    
    /**
     * @brief The minimum value of this variable.
     * @note Integer/boolean values will always be clamped to the ceiling of this value.
     * @warning If hasMin() is false, the value this function returns is undefined.
     * @return Minimum value of the variable.
     */
    float minValue() const;
    
    /**
     * @brief Sets the minimum value allowed for this variable. The current value is clamped (as a float) if necessary.
     * @param value Value to set.
     */
    void setMinValue(float value);
    
    /**
     * @brief Sets the minimum value allowed for this variable. The current value is clamped (as a float) if necessary.
     * @param info CommandSenderInfo for printing messages.
     * @param value Value to set.
     */
    void setMinValue(CommandSenderInfo &info, float value);
    
    /**
     * @brief Returns whether or not this variable has a max value.
     * @return True if the variable has a max value, false otherwise.
     */
    bool hasMax() const;
    
    /**
     * @brief Sets whether this variable has a maximum value.
     * @note If true is passed and the current value is greater than the max, it will
     * be clamped according to the float clamping rules.
     * @param b True if variable should have maximum value, false otherwise.
     */
    void setHasMax(bool b);
    
    /**
     * @brief Sets whether this variable has a maximum value.
     * @note If true is passed and the current value is greater than the max, it will
     * be clamped according to the float clamping rules.
     * @param info CommandSenderInfo for printing messages.
     * @param b True if variable should have maximum value, false otherwise.
     */
    void setHasMax(CommandSenderInfo &info, bool b);
    
    /**
     * @brief The maximum value of this variable.
     * @note Integer/boolean values will always be clamped to the floor of this value.
     * @warning If hasMax() is false, the value this function returns is undefined.
     * @return Maximum value of the variable.
     */
    float maxValue() const;
    
    /**
     * @brief Sets the maximum value allowed for this variable. The current value is clamped (as a float) if necessary.
     * @param value Value to set.
     */
    void setMaxValue(float value);
    
    /**
     * @brief Sets the maximum value allowed for this variable. The current value is clamped (as a float) if necessary.
     * @param info CommandSenderInfo for printing messages.
     * @param value Value to set.
     */
    void setMaxValue(CommandSenderInfo &info, float value);
    
    /**
     * @brief Returns the default value of this variable.
     * @return Default value.
     */
    QString defaultValue() const;
    
    /**
     * @brief Sets the variable to its default value.
     */
    void setToDefault();
    
    /**
     * @brief Gets the value of the variable as a string.
     * @return Value of variable as a string.
     */
    QString stringValue() const;
    
    /**
     * @brief Sets the value of the variable as a string.
     * @note If the variable has a min or max, this function will fail if the string cannot be converted
     * to a numerical value.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    QString setValue(const QString &val);
    
    /**
     * @brief Sets the value of the variable as a string.
     * @note If the variable has a min or max, this function will fail if the string cannot be converted
     * to a numerical value.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    QString setValue(const char* val);
    
    /**
     * @brief Sets the value of the variable as a string.
     * @note If the variable has a min or max, this function will fail if the string cannot be converted
     * to a numerical value.
     * @param info CommandSenderInfo for printing messages.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    QString setValue(const CommandSenderInfo &info, const QString &val);
    
    /**
     * @brief Gets the value of the variable as an integer.
     * @return Value of variable as an integer.
     */
    int intValue() const;
    
    /**
     * @brief Sets the value of the variable as an integer.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    int setValue(int val);
    
    /**
     * @brief Sets the value of the variable as an integer.
     * @param info CommandSenderInfo for printing messages.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    int setValue(CommandSenderInfo &info, int val);
    
    /**
     * @brief Gets the value of the variable as a float.
     * @return Value of variable as a float.
     */
    float floatValue() const;
    
    /**
     * @brief Sets the value of the variable as a float.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    float setValue(float val);
    
    /**
     * @brief Sets the value of the variable as a float.
     * @param info CommandSenderInfo for printing messages.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    float setValue(CommandSenderInfo &info, float val);
    
    /**
     * @brief Gets the value of the variable as a boolean.
     * @return Value of variable as a boolean.
     */
    bool boolValue() const;
    
    /**
     * @brief Sets the value of the variable as a boolean.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    bool setValue(bool val);
    
    /**
     * @brief Sets the value of the variable as a boolean.
     * @param info CommandSenderInfo for printing messages.
     * @param val Value to set.
     * @return Eventual value that the variable was set to.
     */
    bool setValue(CommandSenderInfo &info, bool val);
    
    /**
     * @brief Returns whether the range between the variable's min and max values permits
     * integer values to be set.
     * @return True if the bounds are not too close together to set an int, false otherwise.
     */
    bool canSetInt() const;
    
    /**
     * @brief Overwrites all flags on the command.
     * @param flags Integer containing the exact state of all flags to set.
     */
    virtual void setFlagsRaw(NGlobalCmd::CMDFLAGS flags);
    
    /**
     * @brief Sets the given flag(s) on the command.
     * @param flag Flag(s) to set.
     */
    virtual void setFlag(NGlobalCmd::CMDFLAGS flag);
    
    /**
     * @brief Toggles the given flag(s) on the command.
     * @param flag Flag(s) to toggle.
     */
    virtual void toggleFlag(NGlobalCmd::CMDFLAGS flag);
    
private:
    
    /**
     * @brief If the minimum bound is greater than the maximum bounds, swaps the values of the inputs.
     * @param min Minimum bound.
     * @param max Maximum bound.
     */
    void validateBounds(float &min, float &max);
    
    /**
     * @brief Clamps the value according to the current min and max. If the value is greater than the max, the max is returned;
     * if the value is less than the min, the min is returned; otherwise, the value is returned.
     * @param value Value to clamp.
     * @return Value as clamped by the variable's min and max bounds.
     */
    float clamp(float value) const;
    
    /**
     * @brief Clamps the value according to the current min and max. If the value is greater than the max, the floor of the
     * max is returned; if the value is less than the min, the ceiling of the min is returned; otherwise, the value is returned.
     * @note This function does not take into account whether the bounds are too close for integer values to be set correctly.
     * @param value Value to clamp.
     * @return Value as clamped by the variable's min and max bounds.
     */
    int clamp(int value) const;
    
    /**
     * @brief Gets the raw string value of the variable.
     * @return Variable's value as a string.
     */
    QString get() const;
    
    /**
     * @brief Sets the raw string value of the variable.
     * @param value Value to set.
     * @return Eventual value that the variable was set to.
     */
    QString set(const QString &value);
    
    /**
     * @brief Sets the raw string value of the variable.
     * @param info CommandSenderInfo for printing messages.
     * @param value Value to set.
     * @return Eventual value that the variable was set to.
     */
    QString set(const CommandSenderInfo &info, const QString &value);
    
    NGlobalCmd::VarCallback     m_pVarCallback; /**< Pointer to the callback that this variable has, if any. */
    QVariant                    m_Variable;     /**< Variant that holds the variable's value. */
    QVariant                    m_Default;      /**< Default value of the variable. */
    bool                        m_bHasMin;      /**< Whether the variable has a min value. */
    bool                        m_bHasMax;      /**< Whether the variable has a max value. */
    float                       m_flMinVal;     /**< Min value of the variable. */
    float                       m_flMaxVal;     /**< Max value of the variable. */
};

#endif // CONVAR_H

/**@}*/
