/*! \file baseproperty.h
 * \brief Defines the property base class and interfaces.
 *
 * The property classes hold a key and a value - these are underlyingly represented as strings and BaseProperty subclasses
 * (defined in property.h) deal with binding these solidly to interfaces for a specific type. A property's key should endeavour to be unique
 * (it is not defined which property will be selected if two properties have the same key). <br> <br>
 * A technical note: due to the Qt limitation that only one inheritance of the QObject class is allowed for any derived class (slots requiring an interface
 * inherit from QObject), slots on BaseProperty classes are exposed through a wrapper returned by BaseProperty::slotsBase(). Connecting a signal to a slot
 * on this wrapper will call the relevant non-slot function on the BaseProperty class.
 */

#ifndef BASEPROPERTY_H
#define BASEPROPERTY_H

#include <QObject>
#include <QPair>

/**
 * \defgroup PropertyClasses Property classess
 * @{
 */

//! Property type identifiers.

//! Used to classify what data type a property holds.
//! When subclassing a new property, define a new identifier here.
enum Property_t
{
    Prop_None = -1,     /**< Property is an instance of BaseProperty. */
    Prop_String,        /**< Property value holds a string. */
    Prop_Int,           /**< Property value holds an int. */
    Prop_UInt,          /**< Property value holds an unsigned int. */
    Prop_Long,          /**< Property value holds a long. */
    Prop_ULong,         /**< Property value holds an unsigned long. */
    Prop_Float,         /**< Property value holds a float. */
    Prop_Double,        /**< Property value holds a double. */
    Prop_LongLong,      /**< Property value holds a long long (qint64). */
    Prop_ULongLong      /**< Property value holds an unsigned long long (qint64). */
};

class QString;
class WBaseProperty;
class PropertyVariant;

//! Base property interface.

//! Implemented by BaseProperty.
//! To access slots, use BaseProperty::slotsBase().
class IBaseProperty
{
public:
    virtual ~IBaseProperty() {}

    /**
     * @brief Returns the property key as a string.
     * @return QString representing the property's key.
     */
    virtual QString     getKey() const = 0;

    /**
     * @brief Returns the property value as a raw string.
     * @return  QString representation of the value the property holds.
     */
    virtual QString     getValueRaw() const = 0;

    /**
     * @brief Returns the comment value as a raw string (without "//").
     * @return Unformatted comment string.
     */
    virtual QString     getComment() const = 0;

    /**
     * @brief Sets the property key as a string. NOTE: Should be unique.
     * @param key The key to set.
     */
    virtual void        setKey(QString key) = 0;

    /**
     * @brief Sets the property value as a raw string, ignoring the intended property format.
     * @param value String value to set.
     */
    virtual void        setValueRaw(QString value) = 0;

    /**
     * @brief Sets the comment value as a raw string (without "//").
     * @param value Comment string to set.
     */
    virtual void        setComment(QString value) = 0;

    /**
    @brief Returns the Property_t to classify this property.
    @return Property_t classifier.
    */
    virtual Property_t getPropertyType() const = 0;

    /**
     * @brief Sets the relevant value in the passed variant to this property's value.
     * @param variant Variant in which to set the value.
     */
    virtual void setVariant(PropertyVariant* variant) = 0;
};
/*!
\def IBaseProperty_iid
\brief Unique ID for IBaseProperty interface.
*/
#define IBaseProperty_iid "Crowbar.Interfaces.IBaseProperty"
Q_DECLARE_INTERFACE(IBaseProperty, IBaseProperty_iid)

//! Base property interface wrapper.

//! To avoid multiple QObject inheritance errors, defining slots in interfaces requires
//! the use of a wrapper object. To connect a signal to a slot on a BaseProperty object,
//! call BaseProperty::slotsBase() to return a pointer to this wrapper, then choose the desired
//! slot.
class WBaseProperty : public QObject, public virtual IBaseProperty
{
    Q_OBJECT
    Q_INTERFACES(IBaseProperty)
public:
    /**
     * @brief Constructor. Pass the BaseProperty we are becoming a wrapper for.
     * @param parent BaseProperty to wrap (object, not the interface).
     */
    WBaseProperty(QObject* parent = 0) : QObject(parent)
    {
        Q_ASSERT(parent);
        m_Interface = qobject_cast<IBaseProperty*>(parent);
        Q_ASSERT(m_Interface);
    }

    /**
     * @brief Returns the property key as a string.
     * @return QString representing the property's key.
     */
    QString getKey() const              { return m_Interface->getKey(); }

    /**
     * @brief Returns the property value as a raw string.
     * @return  QString representation of the value the property holds.
     */
    QString getValueRaw() const         { return m_Interface->getValueRaw(); }

    /**
     * @brief Returns the comment value as a raw string (without "//").
     * @return Unformatted comment string.
     */
    QString getComment() const          { return m_Interface->getComment(); }

    /**
    @brief Returns the Property_t to classify this property.
    @return Property_t classifier.
    */
    Property_t getPropertyType() const { return m_Interface->getPropertyType(); }

public slots:
    /**
     * @brief Sets the property key as a string. NOTE: Should be unique.
     * @param key The key to set.
     */
    void    setKey(QString key)         { m_Interface->setKey(key); }

    /**
     * @brief Sets the property value as a raw string, ignoring the intended property format.
     * @param value String value to set.
     */
    void    setValueRaw(QString value)  { m_Interface->setValueRaw(value); }

    /**
     * @brief Sets the comment value as a raw string (without "//").
     * @param value Comment string to set.
     */
    void    setComment(QString value)   { m_Interface->setComment(value); }

    /**
     * @brief Sets the relevant value in the passed variant to this property's value.
     * @param variant Variant in which to set the value.
     */
    void setVariant(PropertyVariant* variant)   { m_Interface->setVariant(variant); }

protected:
    IBaseProperty*  m_Interface;    /**< The interface whose functions we bind to. */
};

//! Base property class.

//! Base property class defining the basic property functions.
//! Although this property deals with QStrings only, a formal StringProperty is available
//! and recommended for use with strings.
class BaseProperty : public QObject, public virtual IBaseProperty
{
    Q_OBJECT
    Q_INTERFACES(IBaseProperty)
public:
    /**
     * @brief If key, value and comment are unspecified, they default to empty strings.
     * @param parent Parent object for this property.
     * @param key Property key (should be unique).
     * @param value Property value.
     * @param comment Optional comment for if we are printing this property into a file.
     */
    explicit BaseProperty(QObject *parent = 0, QString key = "", QString value = "",
                          QString comment = "");

    /**
     * @brief Returns the interface wrapper to allow binding to slots on this property.
     * @return WBaseProperty interface wrapper.
     */
    WBaseProperty* slotsBase() const { return m_WInterface; }

    /**
    @brief Returns the Property_t to classify this property.
    @return Property_t classifier.
    */
    virtual inline Property_t getPropertyType() const { return Prop_None; }

    /**
     * @brief Returns the property key as a string.
     * @return QString representing the property's key.
     */
    virtual QString getKey() const;

    /**
     * @brief Returns the property value as a raw string.
     * @return  QString representation of the value the property holds.
     */
    virtual QString getValueRaw() const;

    /**
     * @brief Returns the comment value as a raw string (without "//").
     * @return Unformatted comment string.
     */
    virtual QString getComment() const;

    /**
     * @brief Sets the property key as a string. NOTE: Should be unique.
     * @param key The key to set.
     */
    virtual void setKey(QString key);

    /**
     * @brief Sets the property value as a raw string, ignoring the intended property format.
     * @param value String value to set.
     */
    virtual void setValueRaw(QString value);

    /**
     * @brief Sets the comment value as a raw string (without "//").
     * @param value Comment string to set.
     */
    virtual void setComment(QString value);

    /**
     * @brief Sets the relevant value in the passed variant to this property's value.
     * @param variant Variant in which to set the value.
     */
    virtual void setVariant(PropertyVariant* variant);
    
protected:
    QPair<QString, QString> m_Property; /**< Property key and value. */
    QString                 m_Comment;  /**< Optional property comment. */

private:
    WBaseProperty*  m_WInterface;       /**< Wrapper to IBaseProperty interface. */
};

/**
 * @brief Interface implemented by PropertyVariant.
 */
class IPropertyVariant
{
public:
    ~IPropertyVariant() {}

    /**
     * @brief Resets all values in the variant to null values.
     */
    virtual void clean() = 0;

    /**
     * @brief Sets the string value in the variant.
     * @param value Value.
     */
    virtual void setString(QString value) = 0;

    /**
     * @brief Gets the string value from the variant.
     * @return String value.
     */
    virtual QString getString() = 0;
};
/*!
    \def IPropertyVariant_iid
    \brief Unique ID for IPropertyVariant interface.
*/
#define IPropertyVariant_iid "Crowbar.Interfaces.IPropertyVariant"
Q_DECLARE_INTERFACE(IPropertyVariant, IPropertyVariant_iid)

/**
 * @brief PropertyVariant interface wrapper.
 */
class WPropertyVariant : public QObject, public virtual IPropertyVariant
{
    Q_OBJECT
    Q_INTERFACES(IPropertyVariant)
public:
    /**
     * @brief Constructor. Pass the BaseProperty we are becoming a wrapper for.
     * @param parent BaseProperty to wrap (object, not the interface).
     */
    WPropertyVariant(QObject* parent = 0) : QObject(parent)
    {
        Q_ASSERT(parent);
        m_Interface = qobject_cast<IPropertyVariant*>(parent);
        Q_ASSERT(m_Interface);
    }

    /**
     * @brief Gets the string value from the variant.
     * @return String value.
     */
    QString getString() { return m_Interface->getString(); }

public slots:
    /**
     * @brief Resets all values in the variant to null values.
     */
    virtual void clean()    { m_Interface->clean(); }

    /**
     * @brief Sets the string value in the variant.
     * @param value Value.
     */
    void setString(QString value)   { m_Interface->setString(value); }

private:
    IPropertyVariant* m_Interface;
};

//! \def DECLARE_VARIANT_GETMETHOD(_methodname, _type)
//! \brief MACRO METHOD: Gets variant _type value.
//!
//! When declaring this method, be sure to use DECLARE_VARIANT_TYPE inside the class for the
//! desired data type.
//! \param _methodname Name of the function that will be available to call in code.
//! \param _type Data type that the method returns.
//! \return Value as _type.
#define DECLARE_VARIANT_GETMETHOD(_methodname, _type) \
_type _methodname() const { return m_##_type; }

//! \def DECLARE_VARIANT_SETMETHOD(_methodname, _type)
//! \brief MACRO METHOD: Sets variant _type value.
//!
//! When declaring this method, be sure to use DECLARE_VARIANT_TYPE inside the class for the
//! desired data type.
//! \param _methodname Name of the function that will be available to call in code.
//! \param _type Data type that the method passes.
#define DECLARE_VARIANT_SETMETHOD(_methodname, _type) \
void _methodname(_type value) { m_##_type = value; }

//! \def DECLARE_VARIANT_TYPE(_type)
//! \brief MACRO METHOD: Declares a member variable of the specified type, to be used with
//! get and set methods.
//! \note It is only possible to declare one member for any specific data type. Further declarations will simply re-declare the same member.
//! \param _type Data type to declare.
#define DECLARE_VARIANT_TYPE(_type) \
_type m_##_type;

//! \def CLEAN_VARIANT_TYPE(_type, _value)
//! \brief MACRO METHOD: Specifies the safe default value to set a data member to when the variant is cleaned. The type should have been
//! declared with DECLARE_VARIANT_TYPE.
//! \param _type Data type to clean.
//! \param _value Value to set the data member to.
#define CLEAN_VARIANT_TYPE(_type, _value) \
m_##_type = _value;

/**
 * @brief Allows fetching of subclassed properties from a list of BaseProperty pointers.
 *
 * The PropertyVariant allows for an implementation of the visitor design pattern: given a list of BaseProperty pointers,
 * passing the variant to a property will allow that property to record its data value in the variant as a specific data type
 * (as opposed to simply a string). This means a typed property can have its data returned as that type without a cast from a
 * BaseProperty pointer to the derived pointer.
 */
class PropertyVariant : public QObject, public virtual IPropertyVariant
{
    Q_OBJECT
    Q_INTERFACES(IPropertyVariant)
public:
    /**
     * @brief Constructor.
     * @param parent Parent object (usually NULL).
     */
    explicit PropertyVariant(QObject *parent = 0);

    /**
     * @brief Returns the interface wrapper to allow binding to slots on this property.
     * @return WPropertyVariant interface wrapper.
     */
    WPropertyVariant* slotsBaseVariant() const  { return m_WInterface; }

    /**
     * @brief Resets all values in the variant to null values.
     */
    virtual void clean();

    /**
     * @brief Sets the string value in the variant.
     * @param value Value.
     */
    void setString(QString value) = 0;

    /**
     * @brief Gets the string value from the variant.
     * @return String value.
     */
    QString getString() = 0;

    DECLARE_VARIANT_GETMETHOD(getInt, int)
    DECLARE_VARIANT_GETMETHOD(getUInt, uint)
    DECLARE_VARIANT_GETMETHOD(getLong, long)
    DECLARE_VARIANT_GETMETHOD(getULong, ulong)
    DECLARE_VARIANT_GETMETHOD(getFloat, float)
    DECLARE_VARIANT_GETMETHOD(getDouble, double)
    DECLARE_VARIANT_GETMETHOD(getLongLong, qlonglong)
    DECLARE_VARIANT_GETMETHOD(getULongLong, qulonglong)

public slots:
    DECLARE_VARIANT_SETMETHOD(setInt, int)
    DECLARE_VARIANT_SETMETHOD(setUInt, uint)
    DECLARE_VARIANT_SETMETHOD(setLong, long)
    DECLARE_VARIANT_SETMETHOD(setULong, ulong)
    DECLARE_VARIANT_SETMETHOD(setFloat, float)
    DECLARE_VARIANT_SETMETHOD(setDouble, double)
    DECLARE_VARIANT_SETMETHOD(setLongLong, qlonglong)
    DECLARE_VARIANT_SETMETHOD(setULongLong, qulonglong)

protected:
    WPropertyVariant* m_WInterface; /**< Interface wrapper. */

private:
    DECLARE_VARIANT_TYPE(int)
    DECLARE_VARIANT_TYPE(uint)
    DECLARE_VARIANT_TYPE(long)
    DECLARE_VARIANT_TYPE(ulong)
    DECLARE_VARIANT_TYPE(float)
    DECLARE_VARIANT_TYPE(double)
    DECLARE_VARIANT_TYPE(qlonglong)
    DECLARE_VARIANT_TYPE(qulonglong)
};

/**@}*/

#endif // BASEPROPERTY_H
