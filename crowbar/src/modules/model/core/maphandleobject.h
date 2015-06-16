#ifndef MAPHANDLEOBJECT_H
#define MAPHANDLEOBJECT_H

#include "model_global.h"
#include "changenotifier.h"

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapHandleObject : public ChangeNotifier
{
    Q_OBJECT
    Q_PROPERTY(int handle READ handle WRITE setHandle NOTIFY handleChanged)
public:
    explicit MapHandleObject(QObject *parent = 0);
    virtual ~MapHandleObject();

    static const int INVALID_HANDLE;
    int handle() const;
    void setHandle(int handle);

signals:
    void handleChanged(int);

public slots:

private:
    int m_iHandle;
};

MODEL_END_NAMESPACE

#endif // MAPHANDLEOBJECT_H
