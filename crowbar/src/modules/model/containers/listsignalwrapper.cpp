#include "listsignalwrapper.h"

MODEL_BEGIN_NAMESPACE
CONTAINERS_BEGIN_NAMESPACE

ListSignalWrapper::ListSignalWrapper(ItemSignal &addSignal, ItemSignal &removeSignal, ClearSignal &clearSignal, QObject *parent) :
    QObject(parent)
{
    addSignal = &ListSignalWrapper::emitItemAdded;
    removeSignal = &ListSignalWrapper::emitItemRemoved;
    clearSignal = &ListSignalWrapper::emitItemsCleared;
}

CONTAINERS_END_NAMESPACE
MODEL_END_NAMESPACE
