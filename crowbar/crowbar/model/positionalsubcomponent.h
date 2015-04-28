#ifndef POSITIONALSUBCOMPONENT_H
#define POSITIONALSUBCOMPONENT_H

#include "mapsubcomponent.h"
#include <QBox3D>
#include <QVector3D>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT PositionalSubComponent : public MapSubComponent
{
    Q_OBJECT
public:
    explicit PositionalSubComponent(MapComponent *parent = 0);

    // These are local to the parent component.
    virtual QBox3D boundingBox() const;
    virtual QVector3D centroid() const;

    virtual bool isValid() const;

signals:

public slots:

};

MODEL_END_NAMESPACE

#endif // POSITIONALSUBCOMPONENT_H
