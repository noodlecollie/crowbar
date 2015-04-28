#ifndef MAPWORLD_H
#define MAPWORLD_H

#include "mapcomponent.h"

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapWorld : public MapComponent
{
    Q_OBJECT
public:
    explicit MapWorld(MapDocument* document);
    virtual ~MapWorld();

signals:

public slots:

};

MODEL_END_NAMESPACE

#endif // MAPWORLD_H
