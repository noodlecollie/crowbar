#include "property.h"

// ===== String Property ===== //
StringProperty::StringProperty(QObject *parent, QString key, QString value, QString comment) :
    BaseProperty(parent, key, value, comment)
{
    m_WInterface = new WStringProperty(this);
}

inline QString StringProperty::getValue(bool* success) const
{
    *success = canConvert();
    return getValueRaw();
}

inline void StringProperty::setValue(QString value)
{
    setValueRaw(value);
}

inline bool StringProperty::canConvert() const
{
    return true;
}

IMPLEMENT_NUMPROP(IntProperty, WIntProperty, int, toInt)
IMPLEMENT_NUMPROP(UIntProperty, WUIntProperty, uint, toUInt)
IMPLEMENT_NUMPROP(LongProperty, WLongProperty, long, toLong)
IMPLEMENT_NUMPROP(ULongProperty, WULongProperty, ulong, toULong)
IMPLEMENT_NUMPROP(FloatProperty, WFloatProperty, float, toFloat)
IMPLEMENT_NUMPROP(DoubleProperty, WDoubleProperty, double, toDouble)
IMPLEMENT_NUMPROP(LongLongProperty, WLongLongProperty, qlonglong, toLongLong)
IMPLEMENT_NUMPROP(ULongLongProperty, WULongLongProperty, qulonglong, toULongLong)
