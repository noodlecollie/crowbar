/*! \file property.h
 * \brief DEPRECATED: Use QVariant() instead.
 *
 * Defines more specific typed properties that inherit from BaseProperty. <br>
 * The property classes hold a key and a value - these are underlyingly represented as strings and BaseProperty subclasses
 * deal with binding these solidly to interfaces for a specific type. A property's key should endeavour to be unique
 * (it is not defined which property will be selected if two properties have the same key). <br> <br>
 * A technical note: due to the Qt limitation that only one inheritance of the QObject class is allowed for any derived class (slots requiring an interface
 * inherit from QObject), slots on property classes are exposed through a wrapper returned by calling slotsFor() on the property. Connecting a signal to a slot
 * on this wrapper will call the relevant non-slot function on the property class.
 */

#ifndef PROPERTY_H
#define PROPERTY_H

#if 0
#include "baseproperty.h"

#pragma deprecated(BaseProperty, IntProperty, UIntProperty, LongProperty, ULongProperty, FloatProperty, DoubleProperty, LongLongProperty, ULongLongProperty)

//! \def DECLARE_PROPERTY_INTERFACE(_classname, _wrapper, _type)
//! \brief Declares a property interface with an interface classname, wrapper classname and data type (such as int).
//! \param _classname Name of the interface class.
//! \param _wrapper Name of the interface wrapper - required to avoid multiple QObject inheritance
//! \param _type Variable type getValue returns.

//! \def DECLARE_NUMPROP(_classname, _interface, _wrapper, _type, _proptype, _vnull)
//! \brief Declares a number-type property which QString has the capability to convert to and from, in macro form for convenience.
//! \param _classname Name of the property class.
//! \param _interface Name of the interface it implements.
//! \param _wrapper Name of the wrapper class to wrap the slots of the interface.
//! \param _type Data type returned by getValue (ie. the type the property should hold).
//! \param _proptype The Property_t value used to classify the property (see baseproperty.h).
//! \param _vnull The default value passed into the property's constructor if no value is specified (eg. 0.0 for float).

//! \def IMPLEMENT_NUMPROP(_classname, _wrapper, _type, _cvtfunc, _variantfunc)
//! \brief Implements the functions for a property declared previously with DECLARE_NUMPROP.
//! \param _classname Name of the property class.
//! \param _wrapper Name of its interface wrapper.
//! \param _type Type returned by getValue.
//! \param _cvtfunc Name of QString function (without any associated parantheses) that returns the value string as a _type (eg. toInt).
//! \param _variantfunc Name of function to call in variant to pass value over.

// A NOTE ON THE FOLLOWING DOXYGEN COMMENTS
// These comments must not be indented or have any characters between the end of the previous line and
// the beginning of the next. Being inside a macro definition is making things behave oddly.

/**
 * \defgroup PropertyClasses Property classess
 * @{
 */

#define DECLARE_PROPERTY_INTERFACE(_classname, _wrapper, _type) \
class _wrapper; \
/**
\brief MACRO PROPERTY INTERFACE FOR _type.
\details Interface for property that handles data of type _type.
*/ \
class _classname \
{ \
public: \
    virtual ~_classname() {} \
/**
@brief Gets this property's value as _type.
@param success True if conversion was successful, false otherwise.
@return The property's value.
*/ \
    virtual _type getValue(bool* success) const = 0; \
/**
@brief Sets the property's value as _type.
@param value Value to set.
*/ \
    virtual void setValue(_type value) = 0; \
/**
@brief Returns whether or not the property's value can be successfully converted.
@return True if conversion is possible, false otherwise.
*/ \
    virtual bool canConvert() const = 0; \
/**
@brief Passes the property's value to the variant
@param variant Variant to pass to.
@return True if conversion succeeded, false otherwise.
*/ \
    virtual bool passTo(PropertyVariant* variant) = 0; \
}; \
Q_DECLARE_INTERFACE(_classname, "Crowbar.Interfaces._classname") \
/**
\brief MACRO PROPERTY INTERFACE WRAPPER FOR _classname.
\details Wrapper for the _classname interface that facilitates binding of slots to the main class.
*/ \
class _wrapper : public QObject, public virtual _classname \
{ \
    Q_OBJECT \
    Q_INTERFACES(_classname) \
public: \
/**
@brief Constructor. Pass the object which implements the interface as parent.
@param parent Object who implements _classname and whose functions this wrapper should bind slots to.
*/ \
    _wrapper(QObject* parent = 0) : QObject(parent) \
    { \
        Q_ASSERT(parent); \
        m_Interface = qobject_cast<_classname*>(parent); \
        Q_ASSERT(m_Interface); \
    } \
/**
@brief Gets this property's value as _type.
@param success True if conversion was successful, false otherwise.
@return The property's value.
*/ \
    _type     getValue(bool* success) const { return m_Interface->getValue(success); } \
/**
@brief Returns whether or not the property's value can be successfully converted.
@return True if conversion is possible, false otherwise.
*/ \
    bool        canConvert() const { return m_Interface->canConvert(); } \
public slots: \
/**
@brief Sets the property's value as _type.
@param value Value to set.
*/ \
    void setValue(_type value) { m_Interface->setValue(value); } \
/**
@brief Passes the property's value to the variant
@param variant Variant to pass to.
@return True if conversion succeeded, false otherwise.
*/ \
    bool passTo(PropertyVariant* variant) { return m_Interface->passTo(variant); } \
protected: \
    _classname*  m_Interface; /**< The interface that represents the bound functions on the parent object. */ \
};

#define DECLARE_NUMPROP(_classname, _interface, _wrapper, _type, _proptype, _vnull) \
/**
\brief MACRO PROPERTY FOR _type.
\details Property that handles data of type _type.
*/ \
class _classname : public BaseProperty, public virtual _interface \
{ \
    Q_OBJECT \
    Q_INTERFACES(_interface) \
public: \
/**
@brief Constructor. Parameters not passed are initialised to safe null values.
@param parent Parent object for this property.
@param key Property key (should be unique).
@param value Property value of type _type.
@param comment Optional string comment for when this property is written into a config file.
*/ \
    explicit _classname(QObject *parent = 0, QString key = "", _type value = _vnull, QString comment = ""); \
/**
@brief Returns the Property_t to classify this property.
@return Property_t classifier.
*/  \
    virtual inline Property_t getPropertyType() const { return _proptype; } \
/**
@brief Passes the property's value to the variant
@param variant Variant to pass to.
@return True if conversion succeeded, false otherwise.
*/ \
    virtual bool passTo(PropertyVariant* variant); \
/**
@brief Gets this property's value as _type.
@param success True if conversion was successful, false otherwise.
@return The property's value.
*/ \
    virtual _type       getValue(bool* success) const; \
/**
@brief Returns whether or not the property's value can be successfully converted.
@return True if conversion is possible, false otherwise.
*/ \
    virtual bool        canConvert() const; \
/**
@brief Sets the property's value as _type.
@param value Value to set.
*/ \
    virtual void        setValue(_type value); \
/**
@brief Returns the interface wrapper to allow binding to slots on this property.
@return _wrapper interface wrapper.
*/ \
    virtual _wrapper*   slotsFor() const { return m_WInterface; } \
private: \
    _wrapper*  m_WInterface; /**< Wrapper that exposes slots defined in this property's interface. */ \
};

#define IMPLEMENT_NUMPROP(_classname, _wrapper, _type, _cvtfunc, _variantfunc) \
/**
@brief Constructor. Parameters not passed are initialised to safe null values.
@param parent Parent object for this property.
@param key Property key (should be unique).
@param value Property value of type _type.
@param comment Optional string comment for when this property is written into a config file.
*/ \
_classname::_classname(QObject *parent, QString key, _type value, QString comment) : BaseProperty(parent, key, "", comment) \
{ \
    setValue(value); \
    m_WInterface = new _wrapper(this); \
} \
/**
@brief Gets this property's value as _type.
@param success True if conversion was successful, false otherwise.
@return The property's value.
*/ \
_type _classname::getValue(bool *success) const \
{ \
    return m_Property.second._cvtfunc(success); \
} \
/**
@brief Sets the property's value as _type.
@param value Value to set.
*/ \
void _classname::setValue(_type value) \
{ \
    m_Property.second.number(value); \
} \
/**
@brief Returns whether or not the property's value can be successfully converted.
@return True if conversion is possible, false otherwise.
*/ \
bool _classname::canConvert() const \
{ \
    bool success; \
    getValue(&success); \
    return success; \
} \
/**
@brief Passes the property's value to the variant
@param variant Variant to pass to.
*/ \
bool _classname::passTo(PropertyVariant* variant) \
{ \
    bool success; \
    variant->_variantfunc(getValue(&success)); \
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

/**
 * @brief The StringProperty class
 * Class that handles string properties.
 */
class StringProperty : public BaseProperty, public virtual IStringProperty
{
    Q_OBJECT
    Q_INTERFACES(IStringProperty)
public:
  /**
   * @brief Constructor. Parameters not passed are initialised to safe null values.
   * @param parent Parent object for this property.
   * @param key Property key (should be unique).
   * @param value Property value.
   * @param comment Optional string comment for when this property is written into a config file.
   */
    explicit StringProperty(QObject *parent = 0, QString key = "", QString value = "",
                            QString comment = "");

    /**
     * @brief Returns the interface wrapper to allow binding to slots on this property.
     * @return WStringProperty interface wrapper.
     */
    WStringProperty* slotsFor() const { return m_WInterface; }

    /**
     * @brief Returns the Property_t to classify this property.
     * @return Property_t classifier.
     */
    virtual inline Property_t getPropertyType() const { return Prop_String; }

    /**
     * @brief Gets this property's value as _type.
     * @param success True if conversion was successful, false otherwise.
     * @return The property's value.
     */
    virtual QString     getValue(bool* success) const;

    /**
     * @brief Returns whether or not the property's value can be successfully converted.
     * @return True if conversion is possible, false otherwise.
     */
    virtual bool        canConvert() const;

    /**
     * @brief Sets the property's value as _type.
     * @param value Value to set.
     */
    virtual void        setValue(QString value);

private:
    WStringProperty*  m_WInterface; /**< The interface that represents the bound functions on the parent object. */
};

DECLARE_NUMPROP(IntProperty, IIntProperty, WIntProperty, int, Prop_Int, 0)
DECLARE_NUMPROP(UIntProperty, IUIntProperty, WUIntProperty, uint, Prop_UInt, 0)
DECLARE_NUMPROP(LongProperty, ILongProperty, WLongProperty, long, Prop_Long, 0)
DECLARE_NUMPROP(ULongProperty, IULongProperty, WULongProperty, ulong, Prop_ULong, 0)
DECLARE_NUMPROP(FloatProperty, IFloatProperty, WFloatProperty, float, Prop_Float, 0.0)
DECLARE_NUMPROP(DoubleProperty, IDoubleProperty, WDoubleProperty, double, Prop_Double, 0.0)
DECLARE_NUMPROP(LongLongProperty, ILongLongProperty, WLongLongProperty, qlonglong, Prop_LongLong, 0)
DECLARE_NUMPROP(ULongLongProperty, IULongLongProperty, WULongLongProperty, qulonglong, Prop_ULongLong, 0)

/**@}*/

#endif // IF_0
#endif // PROPERTY_H
