#ifndef HELPER_MACROS_H
#define HELPER_MACROS_H

// IMPORTANT NOTE: ALWAYS fully qualify namespaces when interfacing with QML!!

// Only works with pointers to objects derived from QObject.
// The macro type argument itsef should not be specified as a pointer.
// Eg. if the existing list holds values of type QObject*,
// pass QObject in here as the type.
#define DECLARE_QML_LIST_TYPE(_type_nopointer, _name) \
    public: \
    QQmlListProperty<_type_nopointer> _name(); \
    private:

#define IMPLEMENT_QML_LIST_TYPE(_ownerType, _type_nopointer, _name) \
    static void _name##QmlAppendFunction(QQmlListProperty<_type_nopointer> *property, _type_nopointer * value) \
    { static_cast<_ownerType*>(property->object)->_name##Append(value); } \
    static _type_nopointer * _name##QmlAtFunction(QQmlListProperty<_type_nopointer> *property, int index) \
    { return static_cast<_ownerType*>(property->object)->_name##At(index); } \
    static void _name##QmlClearFunction(QQmlListProperty<_type_nopointer> *property) \
    { static_cast<_ownerType*>(property->object)->_name##Clear(); } \
    static int _name##QmlCountFunction(QQmlListProperty<_type_nopointer> *property) \
    { return static_cast<_ownerType*>(property->object)->_name##Count(); } \
    QQmlListProperty<_type_nopointer> _ownerType::_name() \
    { return QQmlListProperty<_type_nopointer>(this, NULL, & _name##QmlAppendFunction, & _name##QmlCountFunction, \
      & _name##QmlAtFunction, & _name##QmlClearFunction); }

// The type here can be a pointer.
#define DECLARE_LIST_TYPE(_type, _name) \
    QVector<_type> m_##_name##List; \
    public: \
    QVector<_type> _name##List() const; \
    _type _name(int index) const; \
    void _name##Append(_type const & item); \
    void _name##RemoveAt(int index); \
    void _name##Remove(_type const & item); \
    void _name##Clear(); \
    _type _name##At(int index) const; \
    int _name##Count() const; \
    bool _name##Contains(_type const & item) const; \
    private:

#define DECLARE_LIST_TYPE_SIGNALS(_type, _name) \
    void _name##Added(_type const &); \
    void _name##Removed(); \
    void _name##Cleared();

#define IMPLEMENT_LIST_TYPE(_ownerType, _type, _name) \
    QVector<_type> _ownerType::_name##List() const \
    { return m_##_name##List; } \
    _type _ownerType::_name(int index) const \
    { return _name##At(index); } \
    void _ownerType::_name##Append(_type const & item) \
    { m_##_name##List.append(item); emit _name##Added(item); } \
    void _ownerType::_name##RemoveAt(int index) \
    { m_##_name##List.remove(index); emit _name##Removed(); } \
    void _ownerType::_name##Remove(_type const & item)\
    { int index = m_##_name##List.indexOf(item); if (index >= 0) _name##RemoveAt(index); } \
    void _ownerType::_name##Clear() \
    { m_##_name##List.clear(); emit _name##Cleared(); } \
    _type _ownerType::_name##At(int index) const \
    { return m_##_name##List.at(index); } \
    int _ownerType::_name##Count() const \
    { return m_##_name##List.count(); } \
    bool _ownerType::_name##Contains(_type const & item) const \
    { return m_##_name##List.contains(item); }

#endif // HELPER_MACROS_H

