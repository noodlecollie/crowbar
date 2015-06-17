#include "qmlclass.h"

// void (*)(QQmlListProperty<T> *property, T *value)
void helperAppendProp(QQmlListProperty<QObject> *property, QObject *value)
{
    static_cast<QMLClass*>(property->object)->appendProp(value);
}

// void (*)(QQmlListProperty<T> *property)
void helperClearProps(QQmlListProperty<QObject> *property)
{
    static_cast<QMLClass*>(property->object)->clearProps();
}

// T *(*)(QQmlListProperty<T> *property, int index)
QObject* helperPropAt(QQmlListProperty<QObject> *property, int index)
{
    return static_cast<QMLClass*>(property->object)->propAt(index);
}

// int (*)(QQmlListProperty<T> *property)
int helperPropCount(QQmlListProperty<QObject> *property)
{
    return static_cast<QMLClass*>(property->object)->propCount();
}

QMLClass::QMLClass(QObject *parent) : QObject(parent)
{
    
}

void QMLClass::appendProp(QObject *obj)
{
    m_PropVector.append(obj);
}

void QMLClass::clearProps()
{
    m_PropVector.clear();
}

QObject* QMLClass::propAt(int index) const
{
    return m_PropVector.at(index);
}

int QMLClass::propCount() const
{
    return m_PropVector.count();
}

QQmlListProperty<QObject> QMLClass::props()
{
    // Append count at clear
    return QQmlListProperty<QObject>(this, NULL, &helperAppendProp, &helperPropCount, &helperPropAt, &helperClearProps);
}
