#ifndef VECTOR_H
#define VECTOR_H

#include "model_global.h"
#include "containers_global.h"
#include <QVector>
#include "listsignalwrapper.h"

MODEL_BEGIN_NAMESPACE
CONTAINERS_BEGIN_NAMESPACE

template<typename T>
class Vector
{
public:
    explicit Vector();
    ~Vector();

    ListSignalWrapper& signalWrapper();

    void append(const T &item);
    void removeAt(int index);
    void remove(const T &item);
    const T& at(int index) const;
    int count() const;
    void clear();
    int indexOf(const T &item) const;
    bool contains(const T &item) const;

private:
    QVector<T> m_Vector;
    ListSignalWrapper::ItemSignal m_pItemAddSignal;
    ListSignalWrapper::ItemSignal m_pItemRemoveSignal;
    ListSignalWrapper::ClearSignal m_pItemClearSignal;
    ListSignalWrapper m_SignalWrapper;
};

template<typename T>
Vector<T>::Vector() :
    m_Vector(), m_pItemAddSignal(NULL), m_pItemRemoveSignal(NULL), m_pItemClearSignal(NULL),
    m_SignalWrapper(m_pItemAddSignal, m_pItemRemoveSignal, m_pItemClearSignal)
{

}

template<typename T>
Vector<T>::~Vector()
{
}

template<typename T>
void Vector<T>::append(const T &item)
{
    m_Vector.append(item);
    m_pItemAddSignal(QVariant::fromValue<T>(item));
}

template<typename T>
const T& Vector<T>::at(int index) const
{
    return m_Vector.at(index);
}

template<typename T>
int Vector<T>::count() const
{
    return m_Vector.count();
}

template<typename T>
void Vector<T>::clear()
{
    m_Vector.clear();
    m_pItemClearSignal();
}

template<typename T>
void Vector<T>::removeAt(int index)
{
    m_pItemRemoveSignal(QVariant::fromValue<T>(at(index)));
    m_Vector.removeAt(index);
}

template<typename T>
void Vector<T>::remove(const T &item)
{
    int index = indexOf(item);
    if ( index >= 0 ) removeAt(index);
}

template<typename T>
int Vector<T>::indexOf(const T &item) const
{
    return m_Vector.indexOf(item);
}

template<typename T>
bool Vector<T>::contains(const T &item) const
{
    return m_Vector.contains(item);
}

CONTAINERS_END_NAMESPACE
MODEL_END_NAMESPACE

#endif // VECTOR_H

