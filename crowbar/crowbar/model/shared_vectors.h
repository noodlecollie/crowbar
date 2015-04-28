 #ifndef EXPOSEDLIST_H
#define EXPOSEDLIST_H

#include "model_global.h"

// Whoop-de-fucking-doo, we can't use Q_OBJECT in macros.
// That means we're going to have to compromise somewhat...

// Everything MOC-related MUST be outside the macros.
// Implement classes like this (copy-paste the desired class type and replace [MyType]):
#if 0
class MODELSHARED_EXPORT ExposedVector_[MyType] : public QObject
{
    Q_OBJECT
    EXPOSED_VECTOR_PROPS([MyType])
signals:
    void itemAdded(const [MyType]&);
    void itemRemoved(const [MyType]&);
};

class MODELSHARED_EXPORT ExposedPVector_[MyType] : public QObject
{
    Q_OBJECT
    EXPOSED_POINTER_VECTOR_PROPS([MyType])
signals:
    void itemAdded([MyType]*);
    void itemRemoved([MyType]*);
};

class MODELSHARED_EXPORT ExposedOVector_[MyType] : public QObject
{
    Q_OBJECT
    EXPOSED_OWNER_VECTOR_PROPS([MyType])
signals:
    void itemAdded([MyType]*);
    void itemRemoved([MyType]*);
};

class MODELSHARED_EXPORT ExposedQPVector_[MyType] : public QObject
{
    Q_OBJECT
    EXPOSED_QPOINTER_VECTOR_PROPS([MyType])
signals:
    void itemAdded([MyType]*);
    void itemRemoved([MyType]*);
};
#endif

#define EXPOSED_VECTOR_PROPS(_type) \
public: \
ExposedVector_##_type(QObject* parent=0):QObject(parent){} \
ExposedVector_##_type(int size, QObject* parent=0):QObject(parent),m_List(size){} \
~ExposedVector_##_type(){clear();} \
inline void prepend(const _type &item ){m_List.prepend(item);emit itemAdded(item);} \
inline void append(const _type &item){m_List.append(item);emit itemAdded(item);} \
inline void insert(int i, const _type &item){m_List.insert(i, item);emit itemAdded(item);} \
inline _type removeFirst(){const _type &i=m_List.front();m_List.pop_front();emit itemRemoved(i);return i;} \
inline _type removeLast(){const _type &i=m_List.back();m_List.pop_back();emit itemRemoved(i);return i;} \
inline _type removeAt(int i){const _type &item=m_List.at(i);m_List.remove(i);emit itemRemoved(item);return item;} \
inline void clear(){while(m_List.count()>0){const _type &i=m_List.back();m_List.pop_back();emit itemRemoved(i);}} \
inline int count() const{return m_List.count();} \
inline const QVector<_type>& constData() const{return m_List;} \
inline const _type& at(int index) const{return m_List.at(index);} \
private: \
QVector<_type> m_List;

// Does not delete pointers on destruction.
#define EXPOSED_POINTER_VECTOR_PROPS(_type) \
public: \
ExposedPVector_##_type(QObject* parent=0):QObject(parent){} \
ExposedPVector_##_type(int size, QObject* parent=0):QObject(parent),m_List(size){} \
~ExposedPVector_##_type(){clear();} \
inline void prepend(_type *item ){m_List.prepend(item);emit itemAdded(item);} \
inline void append(_type *item){m_List.append(item);emit itemAdded(item);} \
inline void insert(int i, _type *item){m_List.insert(i, item);emit itemAdded(item);} \
inline _type* removeFirst(){_type *i=m_List.front();m_List.pop_front();emit itemRemoved(i);return i;} \
inline _type* removeLast(){_type *i=m_List.back();m_List.pop_back();emit itemRemoved(i);return i;} \
inline _type* removeAt(int i){_type *item=m_List.at(i);m_List.remove(i);emit itemRemoved(item);return item;} \
inline void clear(){while(m_List.count()>0){_type *i=m_List.back();m_List.pop_back();emit itemRemoved(i);}} \
inline void deleteAll(){while(m_List.count()>0){_type *i=m_List.back();m_List.pop_back();emit itemRemoved(i);delete i;}} \
inline int count() const{return m_List.count();} \
inline const QVector<_type*>& constData() const{return m_List;} \
inline _type* at(int index){return m_List.at(index);} \
private: \
QVector<_type*> m_List;

// Deletes pointers on destruction.
#define EXPOSED_OWNER_VECTOR_PROPS(_type) \
public: \
ExposedOVector_##_type(QObject* parent=0):QObject(parent){} \
ExposedOVector_##_type(int size, QObject* parent=0):QObject(parent),m_List(size){} \
~ExposedOVector_##_type(){deleteAll();} \
inline void prepend(_type *item ){m_List.prepend(item);emit itemAdded(item);} \
inline void append(_type *item){m_List.append(item);emit itemAdded(item);} \
inline void insert(int i, _type *item){m_List.insert(i, item);emit itemAdded(item);} \
inline _type* removeFirst(){_type *i=m_List.front();m_List.pop_front();emit itemRemoved(i);return i;} \
inline _type* removeLast(){_type *i=m_List.back();m_List.pop_back();emit itemRemoved(i);return i;} \
inline _type* removeAt(int i){_type *item=m_List.at(i);m_List.remove(i);emit itemRemoved(item);return item;} \
inline void clear(){while(m_List.count()>0){_type *i=m_List.back();m_List.pop_back();emit itemRemoved(i);}} \
inline void deleteAll(){while(m_List.count()>0){_type *i=m_List.back();m_List.pop_back();emit itemRemoved(i);delete i;}} \
inline int count() const{return m_List.count();} \
inline const QVector<_type*>& constData() const {return m_List;} \
inline _type* at(int index){return m_List.at(index);} \
private: \
QVector<_type*> m_List;

// Does not delete pointers on destruction.
#define EXPOSED_QPOINTER_VECTOR_PROPS(_type) \
public: \
ExposedQPVector_##_type(QObject* parent=0):QObject(parent){} \
ExposedQPVector_##_type(int size, QObject* parent=0):QObject(parent),m_List(size){} \
~ExposedQPVector_##_type(){clear();} \
inline void prepend(_type *item ){m_List.prepend(QPointer<_type>(item));emit itemAdded(item);} \
inline void append(_type *item){m_List.append(QPointer<_type>(item));emit itemAdded(item);} \
inline void insert(int i, _type *item){m_List.insert(i, QPointer<_type>(item));emit itemAdded(item);} \
inline _type* removeFirst(){_type *i=m_List.front().data();m_List.pop_front();emit itemRemoved(i);return i;} \
inline _type* removeLast(){_type *i=m_List.back().data();m_List.pop_back();emit itemRemoved(i);return i;} \
inline _type* removeAt(int i){_type *item=m_List.at(i).data();m_List.remove(i);emit itemRemoved(item);return item;} \
inline void clear(){while(m_List.count()>0){_type *i=m_List.back().data();m_List.pop_back();emit itemRemoved(i);}} \
inline int count() const{return m_List.count();} \
inline const QVector<QPointer<_type> >& constData() const {return m_List;} \
inline _type* at(int index){return m_List.at(index);} \
private: \
QVector<QPointer<_type> > m_List;

// Stop complaining "no relevant classes found"...
#include <QObject>
#include <QVector>

namespace Vectors
{
    class ExposedVector_char : public QObject
    {
        Q_OBJECT
        EXPOSED_VECTOR_PROPS(char)
    signals:
        void itemAdded(const char&);
        void itemRemoved(const char&);
    };
}

#endif // EXPOSEDLIST_H
