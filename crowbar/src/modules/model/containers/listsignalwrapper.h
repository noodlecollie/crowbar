#ifndef LISTSIGNALWRAPPER_H
#define LISTSIGNALWRAPPER_H

#include "model_global.h"
#include "containers_global.h"
#include <QObject>
#include <QVariant>

MODEL_BEGIN_NAMESPACE
CONTAINERS_BEGIN_NAMESPACE

class MODELSHARED_EXPORT ListSignalWrapper : public QObject
{
    Q_OBJECT
public:
    typedef void (ListSignalWrapper::*ItemSignal)(const QVariant&);
    typedef void (ListSignalWrapper::*ClearSignal)();

    // Pass references in on construction so that the wrapper can inform another class
    // of its private slots, in case declaring a friend class is difficult (eg. templated).
    explicit ListSignalWrapper(ItemSignal &addSignal, ItemSignal &removeSignal, ClearSignal &clearSignal,
                               QObject *parent = 0);

signals:
    void itemAdded(const QVariant&);
    void itemRemoved(const QVariant&);
    void itemsCleared();

public slots:

private slots:
    void emitItemAdded(const QVariant &item) { emit itemAdded(item); }
    void emitItemRemoved(const QVariant &item) { emit itemRemoved(item); }
    void emitItemsCleared() { emit itemsCleared(); }
};

CONTAINERS_END_NAMESPACE
MODEL_END_NAMESPACE

#endif // LISTSIGNALWRAPPER_H
