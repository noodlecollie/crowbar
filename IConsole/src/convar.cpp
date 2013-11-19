#include "wr_convar.h"

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
    
    // If we have a min or max, we should validate the string's numerical value.
    if ( hasMin() || hasMax() )
    {
        // Check whether the string can be cast numerically. If not, we shouldn't accept it.
        bool success;
        float cast = toSet.toFloat(&success);
        
        // Couldn't cast
        if ( !success )
        {
            return m_Variable.toString();
        }
        
        // Ensure our value is clamped.
        toSet = QString::number(clamp(cast));
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
    return set(val);
}

QString ConVar::setValue(const char *val)
{
    return set(val);
}

int ConVar::intValue() const
{
    return m_Variable.toInt();
}

int ConVar::setValue(int val)
{
    // Clamping is done in set().
    //val = clamp(val);
    
    return set(QString::number(val)).toInt();
}

float ConVar::floatValue() const
{
    return m_Variable.toFloat();
}

float ConVar::setValue(float val)
{
    // Clamping is done in set().
    //val = clamp(val);
    
    return set(QString::number(val)).toFloat();
}

bool ConVar::boolValue() const
{
    return (intValue() != 0);
}

bool ConVar::setValue(bool val)
{
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
    // Straight int cast just truncates AFAIK.
    if ( hasMax() && (float)value > getMax() ) value = (int)getMax();
    else if ( hasMin() && (float)value < getMin() ) value = 1 + (int)getMin();
    
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
