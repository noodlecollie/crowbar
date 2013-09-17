#include "wr_convar.h"

ConVar::ConVar(const QString &name, const QString &def, NGlobalCmd::VarCallback callback, const QString &desc, NGlobalCmd::CMDFLAGS flags,
               bool hasMin, float min, bool hasMax, float max, QObject *parent) :
    ListedConsoleCommand(name, desc, flags, parent), m_bHasMax(hasMax), m_bHasMin(hasMin), m_flMaxVal(max), m_flMinVal(min), m_pVarCallback(callback),
    m_Default(def), m_Variable(def)
{
    validateBounds(m_flMinVal, m_flMaxVal);
}

ConVar::ConVar(const QString &name, const QString &def, CommandManager *manager, ListedConsoleCommand *list, NGlobalCmd::VarCallback callback,
               const QString &desc, NGlobalCmd::CMDFLAGS flags, bool hasMin, float min, bool hasMax, float max, QObject *parent) :
    ListedConsoleCommand(name, manager, list, desc, flags, parent), m_bHasMax(hasMax), m_bHasMin(hasMin), m_flMaxVal(max), m_flMinVal(min),
    m_pVarCallback(callback), m_Default(def), m_Variable(def)
{
    validateBounds(m_flMinVal, m_flMaxVal);
}

QString ConVar::set(const QString &value)
{
    // Call our callback (if it exists) before setting the eventual value.
    QString toSet = value;

    if ( m_pVarCallback )
    {
        (m_pVarCallback)(m_Variable.toString(), toSet);
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
    // Hopefully we can get away without using fuzzy compare here?
    if ( min > max )
    {
        float temp = min;
        min = max;
        max = temp;
    }
}

QString ConVar::getConVarString() const
{
    return get();
}

QString ConVar::setConVar(const QString &val)
{
    return set(val);
}

int ConVar::getConVarInt() const
{
    return m_Variable.toInt();
}

int ConVar::setConVar(int val)
{
    // Clamp - make sure the integer is inside our bounds.
    if ( hasMax() && (float)val > getMax() ) val = (int)getMax();
    else if ( hasMin() && (float)val < getMin() ) val = 1 + (int)getMin();
    
    return set(QString::number(val)).toInt();
}

float ConVar::getConVarFloat() const
{
    return m_Variable.toFloat();
}

float ConVar::setConVar(float val)
{
    // Clamp
    if ( hasMax() && val > getMax() ) val = getMax();
    else if ( hasMin() && val < getMin() ) val = getMin();
    
    return set(QString::number(val)).toFloat();
}

bool ConVar::getConVarBool() const
{
    return (getConVarInt() != 0);
}

bool ConVar::setConVar(bool val)
{
    // Anything other than zero should be true!
    return (set(QString::number(val)).toInt() != 0);
}
