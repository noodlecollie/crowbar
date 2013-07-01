#ifndef PROPERTY_H
#define PROPERTY_H

#include "baseproperty.h"

// I think a macro's justified here, otherwise it's a massive pain in the arse.
// _classname   Name of the interface class.
// _wrapper     Name of the interface wrapper - required to avoid multiple QObject inheritance
// _type        Variable type getValue returns.
#define DECLARE_PROPERTY_INTERFACE(_classname, _wrapper, _type) \
class _wrapper; \
class _classname \
{ \
public: \
    virtual ~_classname() {} \
    virtual Property_t getPropertyType() const = 0; \
    virtual _type getValue(bool* success) const = 0; \
    virtual void setValue(_type value) = 0; \
    virtual bool canConvert() const = 0; \
    virtual _wrapper* slotsFor() const = 0; \
}; \
Q_DECLARE_INTERFACE(_classname, "Crowbar.Interfaces._classname") \
class _wrapper : public QObject, public virtual _classname \
{ \
    Q_OBJECT \
    Q_INTERFACES(_classname) \
public: \
    _wrapper(QObject* parent = 0) : QObject(parent) \
    { \
        Q_ASSERT(parent); \
        m_Interface = qobject_cast<_classname*>(parent); \
        Q_ASSERT(m_Interface); \
    } \
    Property_t  getPropertyType() const { return m_Interface->getPropertyType(); } \
    _type     getValue(bool* success) const { return m_Interface->getValue(success); } \
    bool        canConvert() const { return m_Interface->canConvert(); } \
    _wrapper* slotsFor() const { return m_Interface->slotsFor(); } \
public slots: \
    void setValue(_type value) { return m_Interface->setValue(value); } \
protected: \
    _classname*  m_Interface; \
};

// Declares a number-based property that QString is capable of natively converting to.
// (ie. there is a QString_to[number]() function)
// _classname   Name of the property class.
// _interface   Name of its interface.
// _wrapper     Name of its interface wrapper.
// _type        Type returned by getValue.
// _proptype    Type returned by getPropertyType.
// _vnull       Null value to pass in constructor (eg. 0.0 for floats).
#define DECLARE_NUMPROP(_classname, _interface, _wrapper, _type, _proptype, _vnull) \
class _classname : public BaseProperty, public virtual _interface \
{ \
    Q_OBJECT \
    Q_INTERFACES(_interface) \
public: \
    explicit _classname(QObject *parent = 0, QString key = "", _type value = _vnull, QString comment = ""); \
    virtual inline Property_t getPropertyType() const { return _proptype; } \
    virtual _type       getValue(bool* success) const; \
    virtual bool        canConvert() const; \
    virtual void        setValue(_type value); \
    virtual _wrapper*   slotsFor() const { return m_WInterface; } \
private: \
    _wrapper*  m_WInterface; \
};

// _classname   Name of the property class.
// _wrapper     Name of its interface wrapper.
// _type        Type returned by getValue.
// _cvtfunc     Name of QString function that returns the value string as a _type.
#define IMPLEMENT_NUMPROP(_classname, _wrapper, _type, _cvtfunc) \
    _classname::_classname(QObject *parent, QString key, _type value, QString comment) : BaseProperty(parent, key, "", comment) \
{ \
    setValue(value); \
    m_WInterface = new _wrapper(this); \
} \
_type _classname::getValue(bool *success) const \
{ \
    return m_Property.second._cvtfunc(success); \
} \
void _classname::setValue(_type value) \
{ \
    m_Property.second.number(value); \
} \
bool _classname::canConvert() const \
{ \
    bool success; \
    getValue(&success); \
    return success; \
}

DECLARE_PROPERTY_INTERFACE(IStringProperty, WStringProperty, QString)
DECLARE_PROPERTY_INTERFACE(IIntProperty, WIntProperty, int)
DECLARE_PROPERTY_INTERFACE(IUIntProperty, WUIntProperty, uint)
DECLARE_PROPERTY_INTERFACE(ILongProperty, WLongProperty, long)
DECLARE_PROPERTY_INTERFACE(IULongProperty, WULongProperty, ulong)
DECLARE_PROPERTY_INTERFACE(IFloatProperty, WFloatProperty, float)
DECLARE_PROPERTY_INTERFACE(IDoubleProperty, WDoubleProperty, double)
DECLARE_PROPERTY_INTERFACE(ILongLongProperty, WLongLongProperty, qlonglong)
DECLARE_PROPERTY_INTERFACE(IULongLongProperty, WULongLongProperty, qulonglong)

// String property.
class StringProperty : public BaseProperty, public virtual IStringProperty
{
    Q_OBJECT
    Q_INTERFACES(IStringProperty)
public:
    explicit StringProperty(QObject *parent = 0, QString key = "", QString value = "",
                            QString comment = "");

    WStringProperty* iStringProperty() const { return m_WInterface; }

    virtual inline Property_t getPropertyType() const { return Prop_String; }
    virtual QString     getValue(bool* success) const;
    virtual bool        canConvert() const;
    virtual void        setValue(QString value);

private:
    WStringProperty*  m_WInterface;
};

DECLARE_NUMPROP(IntProperty, IIntProperty, WIntProperty, int, Prop_Int, 0)
DECLARE_NUMPROP(UIntProperty, IUIntProperty, WUIntProperty, uint, Prop_UInt, 0)
DECLARE_NUMPROP(LongProperty, ILongProperty, WLongProperty, long, Prop_Long, 0)
DECLARE_NUMPROP(ULongProperty, IULongProperty, WULongProperty, ulong, Prop_ULong, 0)
DECLARE_NUMPROP(FloatProperty, IFloatProperty, WFloatProperty, float, Prop_Float, 0.0)
DECLARE_NUMPROP(DoubleProperty, IDoubleProperty, WDoubleProperty, double, Prop_Double, 0.0)
DECLARE_NUMPROP(LongLongProperty, ILongLongProperty, WLongLongProperty, qlonglong, Prop_LongLong, 0)
DECLARE_NUMPROP(ULongLongProperty, IULongLongProperty, WULongLongProperty, qulonglong, Prop_ULongLong, 0)

#endif // PROPERTY_H
