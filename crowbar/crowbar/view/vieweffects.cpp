#include "vieweffects.h"
#include <QGLAbstractEffect>
#include "effects/selectionboxeffect.h"
#include "effects/colourblendedtextureeffect.h"

VIEW_BEGIN_NAMESPACE

namespace Effects
{
    static QGLAbstractEffect* effectList[TOTAL_EFFECTS];
    
    void initialiseEffectList()
    {
        memset(effectList, 0, sizeof(QGLAbstractEffect*));
        
        effectList[SelectionBox] = new SelectionBoxEffect();
        effectList[ColourBlendedTexture] = new ColourBlendedTextureEffect();
    }
    
    QGLAbstractEffect* presetEffect(EffectId effect)
    {
        static bool initialised = false;
        if ( !initialised )
        {
            initialiseEffectList();
            initialised = true;
        }
        
        if ( effect < 0 || effect >= TOTAL_EFFECTS ) return NULL;
        return effectList[effect];
    }
}

VIEW_END_NAMESPACE
