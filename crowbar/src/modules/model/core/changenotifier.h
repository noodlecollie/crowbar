#ifndef CHANGENOTIFIER_H
#define CHANGENOTIFIER_H

#include "model_global.h"
#include <QObject>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT ChangeNotifier : public QObject
{
    Q_OBJECT
public:
    explicit ChangeNotifier(QObject *parent = 0);
    virtual ~ChangeNotifier();

    enum ChangeFlag
    {
        FlagRender = 0x1,       // Render data has changed.

        FlagAll = 0xFFFFFFFF    // Full update!
    };
    Q_DECLARE_FLAGS(ChangeFlags, ChangeFlag)
    Q_FLAG(ChangeFlags)

signals:
    void changed(ChangeFlags, int);

protected:
    void generateChange(ChangeFlags flags);

public slots:

protected slots:
    // Hook owned components up to this in order
    // for the owner's changed() signal to only be
    // emitted once if multiple components change at
    // once from a single event.
    void relayChange(ChangeFlags flags, int id);
};

MODEL_END_NAMESPACE

#endif // CHANGENOTIFIER_H
