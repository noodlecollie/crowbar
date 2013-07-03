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
    Prop_String = 0,    /**< Property value holds a string. */
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
    
protected:
    QPair<QString, QString> m_Property; /**< Property key and value. */
    QString                 m_Comment;  /**< Optional property comment. */

private:
    WBaseProperty*  m_WInterface;       /**< Wrapper to IBaseProperty interface. */
};

/**@}*/

#endif // BASEPROPERTY_H
