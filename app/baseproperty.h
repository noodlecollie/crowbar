#ifndef BASEPROPERTY_H
#define BASEPROPERTY_H

#include <QObject>
#include <QPair>

// Property type identifiers.
// When subclassing a property, define a new identifier here.
enum Property_t
{
    Prop_String = 0,
    Prop_Int,
    Prop_UInt,
    Prop_Long,
    Prop_ULong,
    Prop_Float,
    Prop_Double,
    Prop_LongLong,
    Prop_ULongLong
};

class QString;
class WBaseProperty;

// Base property interface.
class IBaseProperty
{
public:
    virtual ~IBaseProperty() {}

    // Returns the property key as a string.
    virtual QString     getKey() const = 0;

    // Returns the property value as a raw string.
    virtual QString     getValueRaw() const = 0;

    // Returns the comment value as a raw string (without "//").
    virtual QString     getComment() const = 0;

    // Sets the property key as a string.
    virtual void        setKey(QString key) = 0;

    // Sets the property value as a raw string.
    virtual void        setValueRaw(QString value) = 0;

    // Sets the comment value as a raw string (without "//").
    virtual void        setComment(QString value) = 0;

    // Retrieves the base interface wrapper to deal with slots.
    virtual WBaseProperty* slotsBase() const = 0;
};
#define IBaseProperty_iid "Crowbar.Interfaces.IBaseProperty"
Q_DECLARE_INTERFACE(IBaseProperty, IBaseProperty_iid)

// Base property interface wrapper
class WBaseProperty : public QObject, public virtual IBaseProperty
{
    Q_OBJECT
    Q_INTERFACES(IBaseProperty)
public:
    WBaseProperty(QObject* parent = 0) : QObject(parent)
    {
        Q_ASSERT(parent);
        m_Interface = qobject_cast<IBaseProperty*>(parent);
        Q_ASSERT(m_Interface);
    }

    QString getKey() const              { return m_Interface->getKey(); }
    QString getValueRaw() const         { return m_Interface->getValueRaw(); }
    QString getComment() const          { return m_Interface->getComment(); }
    WBaseProperty* slotsBase() const    { return m_Interface->slotsBase(); }

public slots:
    void    setKey(QString key)         { m_Interface->setKey(key); }
    void    setValueRaw(QString value)  { m_Interface->setValueRaw(value); }
    void    setComment(QString value)   { m_Interface->setComment(value); }

protected:
    IBaseProperty*  m_Interface;
};

// Base property class.
class BaseProperty : public QObject, public virtual IBaseProperty
{
    Q_OBJECT
    Q_INTERFACES(IBaseProperty)
public:
    explicit BaseProperty(QObject *parent = 0, QString key = "", QString value = "",
                          QString comment = "");

    WBaseProperty* slotsBase() const { return m_WInterface; }

    virtual QString getKey() const;
    virtual QString getValueRaw() const;
    virtual QString getComment() const;
    virtual void setKey(QString key);
    virtual void setValueRaw(QString value);
    virtual void setComment(QString value);
    
protected:
    QPair<QString, QString> m_Property; // KV pair this property holds.
    QString                 m_Comment;

private:
    WBaseProperty*  m_WInterface;
};

#endif // BASEPROPERTY_H
