#include "baseproperty.h"
#include <QString>

// ===== Base Property ===== //
BaseProperty::BaseProperty(QObject *parent, QString key, QString value, QString comment) :
    QObject(parent)
{
    m_WInterface = new WBaseProperty(this);
    setKey(key);
    setValueRaw(value);
    setComment(comment);
}

inline QString BaseProperty::getKey() const
{
    return m_Property.first;
}

inline QString BaseProperty::getValueRaw() const
{
    return m_Property.second;
}

inline QString BaseProperty::getComment() const
{
    return m_Comment;
}

inline void BaseProperty::setKey(QString key)
{
    m_Property.first = key;
}

inline void BaseProperty::setValueRaw(QString value)
{
    m_Property.second = value;
}

inline void BaseProperty::setComment(QString value)
{
    m_Comment = value;
}

inline void BaseProperty::setVariant(PropertyVariant *variant)
{
    variant->setString(getValueRaw());
}

// ===== Variant ===== //
PropertyVariant::PropertyVariant(QObject *parent) : QObject(parent)
{
    m_WInterface = new WPropertyVariant(this);
}

void PropertyVariant::clean()
{
    // List variant to clean here.
    CLEAN_VARIANT_TYPE(int, 0)
    CLEAN_VARIANT_TYPE(uint, 0)
    CLEAN_VARIANT_TYPE(long, 0)
    CLEAN_VARIANT_TYPE(ulong, 0)
    CLEAN_VARIANT_TYPE(float, 0.0)
    CLEAN_VARIANT_TYPE(double, 0.0)
    CLEAN_VARIANT_TYPE(qlonglong, 0)
    CLEAN_VARIANT_TYPE(qulonglong, 0)
}
