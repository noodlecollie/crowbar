#ifndef MAPCOMPONENT_H
#define MAPCOMPONENT_H

#include <QObject>
#include "model_global.h"
#include "imaphandle.h"
#include "iselectable.h"

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapComponent : public QObject, public IMapHandle, public ISelectable
{
    Q_OBJECT
    Q_PROPERTY(MapHandle_t handle READ handle WRITE setHandle NOTIFY handleChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedStatusChanged)
public:
    explicit MapComponent(QObject *parent = 0);

public: // IMapHandle
    virtual MapHandle_t handle() const;
    virtual void setHandle(MapHandle_t handle);

public: // ISelectable
    virtual bool selected() const;
    virtual void setSelected(bool isSelected);

signals:
    void handleChanged(MapHandle_t);
    void selectedStatusChanged(bool);

public slots:

private:
    MapHandle_t m_iHandle;
    bool        m_bSelected;
};

MODEL_END_NAMESPACE

#endif // MAPCOMPONENT_H
