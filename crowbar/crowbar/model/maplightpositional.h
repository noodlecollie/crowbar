#ifndef MAPLIGHTPOSITIONAL_H
#define MAPLIGHTPOSITIONAL_H

#include "maplightbase.h"

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT MapLightPositional : public MapLightBase
{
    Q_OBJECT
    MAPCOMPONENT_SUBCLASS(MapLightPositional, MapLightBase)
public:
 
signals:
    
public slots:
    
private slots:
    void handleTransformChanged();
    
private:
    void updateLightPosition();
};

MODEL_END_NAMESPACE

#endif // MAPLIGHTPOSITIONAL_H
