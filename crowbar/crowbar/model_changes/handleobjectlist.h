#ifndef HANDLEOBJECTLIST_H
#define HANDLEOBJECTLIST_H

#include "model_global.h"
#include "imaphandle.h"
#include "imaptitle.h"
#include <QVector>
#include <QObject>
#include <QtDebug>

#define DEFINE_SIGNALS(_type) \
void itemAdded##_type(_type *); \
void itemRemoved##_type(_type *); \
    
#define DEFINE_SIGNAL_TRIGGERS(_type) \
void sendItemAdded(_type * item); \
void sendItemRemoved(_type * item);

#define IMPLEMENT_SIGNAL_TRIGGERS(_type) \
void ObjectListSignals::sendItemAdded(_type * item) \
{ \
emit itemAdded##_type(item); \
} \
void ObjectListSignals::sendItemRemoved(_type * item) \
{ \
emit itemRemoved##_type(item); \
}

MODEL_BEGIN_NAMESPACE

class MapBrush;
class MapVertex;
class MapEdge;
class MapFace;

template<typename T>
class HandleObjectList;

class ObjectListSignals : public QObject
{
    Q_OBJECT
    template<typename T>
    friend class HandleObjectList;
public:
    
signals:
    void titleChanged(const QString&);
    void aboutToClear();
    void cleared();
    
    DEFINE_SIGNALS(MapBrush)
    DEFINE_SIGNALS(MapVertex)
    DEFINE_SIGNALS(MapEdge)
    DEFINE_SIGNALS(MapFace)
private:
    void sendTitleChanged(const QString &title) { emit titleChanged(title); }
    void sendAboutToClear() { emit aboutToClear(); }
    void sendCleared() { emit cleared(); }
    
    DEFINE_SIGNAL_TRIGGERS(MapBrush)
    DEFINE_SIGNAL_TRIGGERS(MapEdge)
    DEFINE_SIGNAL_TRIGGERS(MapVertex)
    DEFINE_SIGNAL_TRIGGERS(MapFace)
};

// Don't use MODELSHARED_EXPORT here because it's a template class - the code
// is generated directly from the template so there's nothing to put in the DLL.

// Ownership policy:
// When an object is added, it is set as a child of the given parent
// and the parent gets ownership of the object.
// When an object is removed, it is removed as a parent and the caller
// gets ownership of the object.
// When the list is cleared, all objects are deleted since there is no
// opportunity for the caller to take ownership of the objects.
template<typename T>
class HandleObjectList : public IMapTitle
{
public:
    typedef void (ObjectListSignals::*HandleObjectEvent) (T*);
    typedef void (ObjectListSignals::*TitleChangeEvent) (const QString&);
    
    ObjectListSignals signalRelay;
    
    HandleObjectList(const QString &title, QObject* parent = 0);
    
    virtual void setTitle(const QString &title);
    virtual QString title() const;

    bool append(T* obj);
    void remove(T* obj);
    T* remove(MapHandle_t handle);
    T* removeAt(int index);
    int indexOf(T* obj) const;
    int indexOf(MapHandle_t handle) const;
    bool contains(T* obj) const;
    bool contains(MapHandle_t handle) const;
    T* at(int index);
    const T* at(int index) const;
    T* byHandle(MapHandle_t handle);
    const T* byHandle(MapHandle_t handle) const;
    int count() const;
    void clear();
    void freeUnusedHandles();
    MapHandle_t nextHandle() const;
    
    //QVector<T*>& itemList();
    const QVector<T*>& constItemList() const;
    
private:
    HandleObjectEvent   m_pAddEvent;
    HandleObjectEvent   m_pRemoveEvent;
    TitleChangeEvent    m_pTitleChangeEvent;
    
private:
    void resetLargestHandle();
    MapHandle_t assignNextHandle();
    void deleteAll();
    
    QVector<T*> m_List;
    MapHandle_t m_iHandleCounter;
    QString     m_szTitle;
    QObject*    m_pParent;
};

template<typename T>
void HandleObjectList<T>::deleteAll()
{
    for ( int i = 0; i < m_List.count(); i++ )
    {
       delete m_List.at(i);
    }
}

template<typename T>
void HandleObjectList<T>::setTitle(const QString &title)
{
    m_szTitle = title;
    if ( m_pTitleChangeEvent ) (signalRelay.*m_pTitleChangeEvent)(m_szTitle);
}

template<typename T>
QString HandleObjectList<T>::title() const
{
    return m_szTitle;
}

template<typename T>
HandleObjectList<T>::HandleObjectList(const QString &title, QObject* parent)
{
    m_pParent = parent;
    m_pAddEvent = &ObjectListSignals::sendItemAdded;
    m_pRemoveEvent = &ObjectListSignals::sendItemRemoved;
    m_pTitleChangeEvent = &ObjectListSignals::sendTitleChanged;
    setTitle(title);
    resetLargestHandle();
}

template<typename T>
bool HandleObjectList<T>::append(T* obj)
{
    if ( nextHandle() == INVALID_HANDLE || !obj ) return false;
    
    m_List.append(obj);
    obj->setParent(m_pParent);
    obj->setHandle(assignNextHandle());
    if ( m_pAddEvent ) (signalRelay.*m_pAddEvent)(obj);
    return true;
}

template<typename T>
T* HandleObjectList<T>::remove(MapHandle_t handle)
{
    int index = indexOf(handle);
    
    if ( index > 0 )
    {
        return removeAt(index);
    }
    
    return NULL;
}

template<typename T>
T* HandleObjectList<T>::removeAt(int index)
{
    T* obj = at(index);
    m_List.removeAt(index);
    obj->setParent(NULL);
    if ( m_pRemoveEvent ) (signalRelay.*m_pRemoveEvent)(obj);
    return obj;
}

template<typename T>
int HandleObjectList<T>::indexOf(T* obj) const
{
    return m_List.indexOf(obj);
}

template<typename T>
int HandleObjectList<T>::indexOf(MapHandle_t handle) const
{
    for ( int i = 0; i < m_List.count(); i++ )
    {
        if ( m_List.at(i)->handle() == handle ) return i;
    }
    
    return -1;
}

template<typename T>
bool HandleObjectList<T>::contains(T *obj) const
{
    return m_List.contains(obj);
}

template<typename T>
bool HandleObjectList<T>::contains(MapHandle_t handle) const
{
    return indexOf(handle) >= 0;
}

template<typename T>
int HandleObjectList<T>::count() const
{
    return m_List.count();
}

template<typename T>
void HandleObjectList<T>::clear()
{
    signalRelay.sendAboutToClear();

    deleteAll();
    m_List.clear();

    signalRelay.sendCleared();
}

template<typename T>
void HandleObjectList<T>::freeUnusedHandles()
{
    // Here we essentially just sequentially renumber the handles in the vector.
    resetLargestHandle();
    
    foreach ( T* obj, m_List )
    {
        obj->setHandle(assignNextHandle());
    }
}

template<typename T>
void HandleObjectList<T>::resetLargestHandle()
{
    m_iHandleCounter = INVALID_HANDLE;
}

template<typename T>
const T* HandleObjectList<T>::at(int index) const
{
    return m_List.at(index);
}

template<typename T>
T* HandleObjectList<T>::at(int index)
{
    return m_List.at(index);
}

template<typename T>
MapHandle_t HandleObjectList<T>::nextHandle() const
{
    // INVALID_HANDLE means none assigned yet.
    if ( m_iHandleCounter == INVALID_HANDLE ) return 0;
    
    // Some handles have been assigned but not the max.
    if ( m_iHandleCounter >= 0 && m_iHandleCounter < MAX_HANDLE)
        return m_iHandleCounter + 1;
    
    // The max handle has been assigned.
    return INVALID_HANDLE;
}

template<typename T>
MapHandle_t HandleObjectList<T>::assignNextHandle()
{
    MapHandle_t next = nextHandle();
    if ( next != INVALID_HANDLE ) m_iHandleCounter = next;
    return next;
}

template<typename T>
const QVector<T*>& HandleObjectList<T>::constItemList() const
{
    return m_List;
}

template<typename T>
T* HandleObjectList<T>::byHandle(int handle)
{
    int index = indexOf(handle);
    if ( index < 0 ) return NULL;
    return at(index);
}

template<typename T>
const T* HandleObjectList<T>::byHandle(int handle) const
{
    int index = indexOf(handle);
    if ( index < 0 ) return NULL;
    return at(index);
}

MODEL_END_NAMESPACE

#endif // HANDLEOBJECTLIST_H
