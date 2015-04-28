#ifndef VIEWEFFECTS_H
#define VIEWEFFECTS_H

#include "view_global.h"

class QGLAbstractEffect;

VIEW_BEGIN_NAMESPACE

namespace Effects
{
    enum EffectId
    {
        SelectionBox = 0,
        ColourBlendedTexture = 1,
        
        TOTAL_EFFECTS
    };
    
    VIEWSHARED_EXPORT QGLAbstractEffect* presetEffect(EffectId effect);
}

VIEW_END_NAMESPACE

#endif // VIEWEFFECTS_H
