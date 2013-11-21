#include "wr_convar.h"
#include <qmath.h>

#define NULL_SENDER_INFO    CommandSenderInfo(name(), NULL, NULL, m_bHasMin, m_flMinVal, m_bHasMax, m_flMaxVal)

ConVar::ConVar(const QString &name, const QString &def, NGlobalCmd::VarCallback callback, const QString &desc, NGlobalCmd::CMDFLAGS flags,
               bool hasMin, float min, bool hasMax, float max, QObject *parent) :
    ListedConsoleCommand(name, desc, flags, parent), m_pVarCallback(callback), m_Variable(def), m_Default(def), m_bHasMin(hasMin),
    m_bHasMax(hasMax), m_flMinVal(min), m_flMaxVal(max)
{
    if ( flagSet(NGlobalCmd::CMDFLAG_ENSURECALLBACK) )
    {
        Q_ASSERT_X(callback, "ConVar::ConVar()", "CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
        
        // If debug is off, un-set the callback flag.
        removeFlag(NGlobalCmd::CMDFLAG_ENSURECALLBACK);
    }
    
    validateBounds(m_flMinVal, m_flMaxVal);
}

ConVar::ConVar(const QString &name, const QString &def, CommandManager *manager, ListedConsoleCommand **list, NGlobalCmd::VarCallback callback,
               const QString &desc, NGlobalCmd::CMDFLAGS flags, bool hasMin, float min, bool hasMax, float max, QObject *parent) :
    ListedConsoleCommand(name, manager, list, desc, flags, parent), m_pVarCallback(callback), m_Variable(def), m_Default(def), m_bHasMin(hasMin),
    m_bHasMax(hasMax), m_flMinVal(min), m_flMaxVal(max)
{
    if ( flagSet(NGlobalCmd::CMDFLAG_ENSURECALLBACK) )
    {
        Q_ASSERT_X(callback, "ConVar::ConVar()", "CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
        
        // If debug is off, un-set the callback flag.
        removeFlag(NGlobalCmd::CMDFLAG_ENSURECALLBACK);
    }
    
    validateBounds(m_flMinVal, m_flMaxVal);
}

QString ConVar::set(const QString &value)
{
    // Construct a null sender info.
    //CommandSenderInfo info = NULL_SENDER_INFO;
    return set(NULL_SENDER_INFO, value);
}

// All set functions should ultimately go through this function.
QString ConVar::set(const CommandSenderInfo &info, const QString &value)
{
    // Don't change if we are read-only.
    if ( flagSet(NGlobalCmd::CMDFLAG_READONLY) ) return m_Variable.toString();
    
    CommandSenderInfo info2 = info;
    
    // We should always pass in our actual min/max values just in case.
    info2.setHasMin(hasMin());
    info2.setMinValue(minValue());
    info2.setHasMax(hasMax());
    info2.setMaxValue(maxValue());

    QString toSet = value;
    
    if ( !m_pVarCallback && flagSet(NGlobalCmd::CMDFLAG_ENSURECALLBACK) )
    {
         info.writeWarning(name() + ": CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
    }

    // Call our callback (if it exists) before setting the eventual value.
    // The callback is allowed to set a value not between the min/max (though this
    // is not recommended).
    if ( m_pVarCallback )
    {
        (m_pVarCallback)(info2, m_Variable.toString(), toSet);
    }

    m_Variable.setValue(toSet);
    return m_Variable.toString();
}

QString ConVar::get() const
{
    return m_Variable.toString();
}

void ConVar::setCallback(NGlobalCmd::VarCallback callback)
{
    if ( flagSet(NGlobalCmd::CMDFLAG_ENSURECALLBACK) && !callback ) return;
    
    m_pVarCallback = callback;
}

NGlobalCmd::VarCallback ConVar::getCallback() const
{
    return m_pVarCallback;
}

bool ConVar::hasMin() const
{
    return m_bHasMin;
}

bool ConVar::hasMax() const
{
    return m_bHasMax;
}

float ConVar::minValue() const
{
    return m_flMinVal;
}

float ConVar::maxValue() const
{
    return m_flMaxVal;
}

void ConVar::setMinValue(float value)
{
    // Set the value.
    m_flMinVal = value;
    validateBounds(m_flMinVal, m_flMaxVal);
    if ( !hasMin() && !hasMax() ) return;
    
    // Check if our current value is still within the bounds.
    // If not, set the clamped value.
    float val = clamp(floatValue());
    if ( !qFuzzyCompare(val, floatValue()) ) setValue(val);
}

void ConVar::setMaxValue(float value)
{
    // Set the value.
    m_flMaxVal = value;
    validateBounds(m_flMinVal, m_flMaxVal);
    if ( !hasMin() && !hasMax() ) return;
    
    // Check if our current value is still within the bounds.
    // If not, set the clamped value.
    float val = clamp(floatValue());
    if ( !qFuzzyCompare(val, floatValue()) ) setValue(val);
}

QString ConVar::defaultValue() const
{
    return m_Default.toString();
}

void ConVar::setToDefault()
{
    set(defaultValue());
}

void ConVar::validateBounds(float &min, float &max)
{
    if ( min > max )
    {
        float temp = min;
        min = max;
        max = temp;
    }
}

QString ConVar::stringValue() const
{
    return get();
}

QString ConVar::setValue(const QString &val)
{
    // Construct a null SenderInfo.
    CommandSenderInfo info = NULL_SENDER_INFO;
    
    // Call setValue with SenderInfo.
    return setValue(info, val);
}

QString ConVar::setValue(const char *val)
{
    // Call standard string setValue.
    return setValue(QString(val));
}

// All QString set functions should ultimately go through this function.
// This applies validation to the string and then calls the private set() function.
QString ConVar::setValue(const CommandSenderInfo &info, const QString &val)
{
    // If we have a min or max, we should validate the string's numerical value.
    if ( hasMin() || hasMax() )
    {
        // Check whether the string can be cast numerically. If not, we shouldn't accept it.
        bool success;
        float cast = val.toFloat(&success);
        
        Q_ASSERT_X(success, "ConVar::setValue()", "String values must be numerical if set when the variable has numerical bounds.");
        
        // Couldn't cast
        if ( !success )
        {
            info.writeWarning(name() + ": String values must be numerical if set when the variable has numerical bounds.");
            return m_Variable.toString();
        }
        
        // Ensure our value is clamped.
        return set(info, QString::number(clamp(cast)));
    }
    
    // Set normally.
    return set(info, val);
}

int ConVar::intValue() const
{
    Q_ASSERT_X(canSetInt(), "ConVar::intValue()", "Min and max bounds are too close together to allow an integer value, returned value is unreliable.");
    info.writeWarning(name() + ": Min and max bounds are too close together to allow an integer value, returned value is unreliable.");
    
    return get().toInt();
}

int ConVar::setValue(int val)
{
    // Construct a null SenderInfo.
    //CommandSenderInfo info = NULL_SENDER_INFO;
    
    return setValue(NULL_SENDER_INFO, val);
}

int ConVar::setValue(CommandSenderInfo &info, int val)
{
    // Check there is at least one integer value between the min and max - if not, return.
    //if ( !canSetInt() ) return intValue();
    Q_ASSERT_X(canSetInt(), "ConVar::setValue()", "Min and max bounds are too close together to be able to set an integer value.");
    info.writeWarning(name() + ": Min and max bounds are too close together to be able to set an integer value.");
    
    // Clamp the value.
    val = clamp(val);
    
    return set(info, QString::number(val)).toInt();
}

float ConVar::floatValue() const
{
    return get().toFloat();
}

float ConVar::setValue(float val)
{
    // Construct a null SenderInfo.
    //CommandSenderInfo info = NULL_SENDER_INFO;
    
    return setValue(NULL_SENDER_INFO, val);
}

float ConVar::setValue(CommandSenderInfo &info, float val)
{
    // Clamp the value.
    val = clamp(val);
    
    return set(info, QString::number(val)).toFloat();
}

bool ConVar::boolValue() const
{
    return (intValue() != 0);
}

bool ConVar::setValue(bool val)
{
    // Construct a null SenderInfo.
    //CommandSenderInfo info = NULL_SENDER_INFO;
    
    return setValue(NULL_SENDER_INFO, val);
}

bool ConVar::setValue(CommandSenderInfo &info, bool val)
{
    // Check there is at least one integer value between the min and max, since we set bools as ints.
    if ( !canSetInt() ) return boolValue();
    
    // Anything other than zero should be true!
    return (set(info, QString::number(val)).toInt() != 0);
}

void ConVar::setHasMin(bool b)
{
    // Set the min.
    m_bHasMin = b;
    if ( !hasMin() && !hasMax() ) return;
    
    // Test the current value against the new bound by clamping it.
    float val = clamp(floatValue());
    
    // If the value was changed, set the new value.
    if ( !qFuzzyCompare(val, floatValue()) ) setValue(val);
}

void ConVar::setHasMax(bool b)
{
    // Set the max.
    m_bHasMax = b;
    if ( !hasMin() && !hasMax() ) return;
    
    // Test the current value against the new bound by clamping it.
    float val = clamp(floatValue());
    
    // If the value was changed, set the new value.
    if ( !qFuzzyCompare(val, floatValue()) ) setValue(val);
}

NGlobalCmd::CmdIdent ConVar::identify() const
{
    return NGlobalCmd::CIVariable;
}

float ConVar::clamp(float value) const
{
    if ( hasMin() && value < minValue() ) value = minValue();
    if ( hasMax() && value < maxValue() ) value = maxValue();
    
    return value;
}

int ConVar::clamp(int value) const
{
    // Make sure the integer is inside our bounds.
    if ( hasMax() && value > qFloor(maxValue()) ) value = qFloor(maxValue());
    else if ( hasMin() && value < qCeil(minValue()) ) value = qCeil(minValue());
    
    return value;
}

void ConVar::setFlagsRaw(NGlobalCmd::CMDFLAGS flags)
{
    if ( (flags & NGlobalCmd::CMDFLAG_ENSURECALLBACK) == NGlobalCmd::CMDFLAG_ENSURECALLBACK )
    {
        // Ensure callback is not null.
        Q_ASSERT_X(m_pVarCallback, "ConVar::setFlagsRaw()", "CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
        info.writeWarning(name() + ": CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
    }
    
    // Call base function.
    ListedConsoleCommand::setFlagsRaw(flags);
}

void ConVar::setFlag(NGlobalCmd::CMDFLAGS flag)
{
    if ( (flag & NGlobalCmd::CMDFLAG_ENSURECALLBACK) == NGlobalCmd::CMDFLAG_ENSURECALLBACK )
    {
        // Ensure callback is not null.
        Q_ASSERT_X(m_pVarCallback, "ConVar::setFlag()", "CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
        info.writeWarning(name() + ": CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
    }
    
    // Call base function.
    ListedConsoleCommand::setFlag(flag);
}

void ConVar::toggleFlag(NGlobalCmd::CMDFLAGS flag)
{
    // If we're toggling the callback flag and it's currently unset (ie. we are setting it):
    if ( (flag & NGlobalCmd::CMDFLAG_ENSURECALLBACK) == NGlobalCmd::CMDFLAG_ENSURECALLBACK && !flagSet(NGlobalCmd::CMDFLAG_ENSURECALLBACK) )
    {
        // Ensure callback is not null.
        Q_ASSERT_X(m_pVarCallback, "ConVar::toggleFlag()", "CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
        info.writeWarning(name() + ": CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
    }
    
    // Call base function.
    ListedConsoleCommand::toggleFlag(flag);
}

bool ConVar::canSetInt() const
{
    // We don't have one or the other bound, return true.
    if ( !hasMax() || !hasMin() ) return true;
    
    // We have a min and a max.
    // If there is no integer number between the bounds (eg 1.7-1.8), we cannot set an integer.
    // This check, however, is not simply whether max-min >= 1, since there is an integer value between the bounds 1.9 and 2.1.
    
    // If the first integer above the min is below the max, there is space to set a clamped int.
    // The converse is also true, but will be captured by this statement as well.
    if ( qCeil(minValue()) <= qFloor(maxValue()) ) return true;
    
    // Otherwise there is no space between the bounds in which to set a clamped int, so return false.
    else return false;
}
