#include "wr_convar.h"
#include <qmath.h>

ConVar::ConVar(const QString &name, const QString &def, NGlobalCmd::VarCallback callback, const QString &desc, NGlobalCmd::CMDFLAGS flags,
               bool hasMin, float min, bool hasMax, float max, QObject *parent) :
    ListedConsoleCommand(name, desc, flags, parent), m_pVarCallback(callback), m_Variable(def), m_Default(def), m_bHasMin(hasMin),
    m_bHasMax(hasMax), m_flMinVal(min), m_flMaxVal(max)
{
    if ( flagSet(NGlobalCmd::CMDFLAG_ENSURECALLBACK) )
    {
        Q_ASSERT_X(callback, "ConVar::ConVar()", "CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
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
    }
    
    validateBounds(m_flMinVal, m_flMaxVal);
}

QString ConVar::set(const QString &value)
{
    // Construct a null sender info.
    CommandSenderInfo info(name(), NULL, NULL, m_bHasMin, m_flMinVal, m_bHasMax, m_flMaxVal);
    return set(info, value);
}

QString ConVar::set(const CommandSenderInfo &info, const QString &value)
{
    // Don't change if we are read-only.
    if ( flagSet(NGlobalCmd::CMDFLAG_READONLY) ) return m_Variable.toString();
    
    CommandSenderInfo info2 = info;
    
    // We should always pass in our actual min/max values.
    info2.setHasMin(m_bHasMin);
    info2.setMinValue(m_flMinVal);
    info2.setHasMax(m_bHasMax);
    info2.setMaxValue(m_flMaxVal);

    QString toSet = value;
    
    // This should be done individually - different types have different clamping rules.
//    // If we have a min or max, we should validate the string's numerical value.
//    if ( hasMin() || hasMax() )
//    {
//        // Check whether the string can be cast numerically. If not, we shouldn't accept it.
//        bool success;
//        float cast = toSet.toFloat(&success);
        
//        // Couldn't cast
//        if ( !success )
//        {
//            return m_Variable.toString();
//        }
        
//        // Ensure our value is clamped.
//        toSet = QString::number(clamp(cast));
//    }

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

float ConVar::getMin() const
{
    return m_flMinVal;
}

float ConVar::getMax() const
{
    return m_flMaxVal;
}

void ConVar::setMin(float value)
{
    m_flMinVal = value;
}

void ConVar::setMax(float value)
{
    m_flMaxVal = value;
}

QString ConVar::getDefault() const
{
    return m_Default.toString();
}

void ConVar::setToDefault()
{
    set(getDefault());
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
    // If we have a min or max, we should validate the string's numerical value.
    if ( hasMin() || hasMax() )
    {
        // Check whether the string can be cast numerically. If not, we shouldn't accept it.
        bool success;
        float cast = val.toFloat(&success);
        
        // Couldn't cast
        if ( !success )
        {
            return m_Variable.toString();
        }
        
        // Ensure our value is clamped.
        toSet = QString::number(clamp(cast));
    }
    
    return set(val);
}

QString ConVar::setValue(const char *val)
{
    return setValue(QString(val));
}

int ConVar::intValue() const
{
    return m_Variable.toInt();
}

int ConVar::setValue(int val)
{
    // Check there is at least one integer value between the min and max - if not, return.
    if ( !canSetInt() ) return intValue();
    
    // Clamp the value.
    val = clamp(val);
    
    return set(QString::number(val)).toInt();
}

float ConVar::floatValue() const
{
    return m_Variable.toFloat();
}

float ConVar::setValue(float val)
{
    // Clamp the value.
    val = clamp(val);
    
    return set(QString::number(val)).toFloat();
}

bool ConVar::boolValue() const
{
    return (intValue() != 0);
}

bool ConVar::setValue(bool val)
{
    // Check there is at least one integer value between the min and max, since we set bools as ints.
    if ( !canSetInt() ) return boolValue;
    
    // Anything other than zero should be true!
    return (set(QString::number(val)).toInt() != 0);
}

NGlobalCmd::CmdIdent ConVar::identify() const
{
    return NGlobalCmd::CIVariable;
}

float ConVar::clamp(float value)
{
    if ( hasMin() && value < getMin() ) value = getMin();
    if ( hasMax() && value < getMax() ) value = getMax();
    
    return value;
}

int ConVar::clamp(int value)
{
    // Make sure the integer is inside our bounds.
    if ( hasMax() && value > qFloor(getMax()) ) value = qFloor(getMax());
    else if ( hasMin() && value < qCeil(getMin()) ) value = qCeil(getMin());
    
    return value;
}

void ConVar::setFlagsRaw(NGlobalCmd::CMDFLAGS flags)
{
    if ( (flags & NGlobalCmd::CMDFLAG_ENSURECALLBACK) == NGlobalCmd::CMDFLAG_ENSURECALLBACK )
    {
        // Ensure callback is not null.
        Q_ASSERT_X(m_pVarCallback, "ConVar::setFlagsRaw()", "CMDFLAG_ENSURECALLBACK requires variable to have a callback.");
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
    }
    
    // Call base function.
    ListedConsoleCommand::toggleFlag(flag);
}

bool ConVar::canSetInt()
{
    // We don't have one or the other bound, return true.
    if ( !hasMax() || !hasMin() ) return true;
    
    // We have a min and a max.
    // If there is no integer number between the bounds (eg 1.7-1.8), we cannot set an integer.
    // This check, however, is not simply whether max-min >= 1, since there is an integer value between the bounds 1.9 and 2.1.
    
    // If the first integer above the min is below the max, there is space to set a clamped int.
    // The converse is also true, but will be captured by this statement as well.
    if ( qCeil(getMin) <= qFloor(getMax()) ) return true;
    
    // Otherwise there is no space between the bounds in which to set a clamped int, so return false.
    else return false;
}
