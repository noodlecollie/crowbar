#include "changenotifier.h"

MODEL_BEGIN_NAMESPACE

ChangeNotifier::ChangeNotifier(QObject *parent) : QObject(parent)
{

}

ChangeNotifier::~ChangeNotifier()
{

}

void ChangeNotifier::generateChange(ChangeFlags flags)
{
    emit changed(flags, qrand());
}

void ChangeNotifier::relayChange(ChangeFlags flags, int id)
{
    static int lastId = -1;

    if ( id != lastId )
    {
        lastId = id;
        emit changed(flags, id);
    }
}

MODEL_END_NAMESPACE
