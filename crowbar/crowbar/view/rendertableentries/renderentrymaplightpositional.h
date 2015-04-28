#ifndef RENDERENTRYMAPLIGHTPOSITIONAL_H
#define RENDERENTRYMAPLIGHTPOSITIONAL_H

#include "rendertableentry.h"
#include "maplightpositional.h"

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT RenderEntryMapLightPositional : public RenderTableEntry
{
    Q_OBJECT
public:
    explicit RenderEntryMapLightPositional(MODEL_NAMESPACE::MapLightPositional* component, MapDocumentView* parent = 0);
    virtual ~RenderEntryMapLightPositional();
    
    inline MODEL_NAMESPACE::MapLightPositional* light() const
    {
        return qobject_cast<MODEL_NAMESPACE::MapLightPositional*>(component());
    }
    
    virtual bool isValid() const { return true; }
    
protected:
    virtual QGLSceneNode* sceneNodeFor(RenderMode mode) const;
    
signals:
    
public slots:
    
private:
    QGLSceneNode* buildWireframeNode() const;
    QGLSceneNode* buildSolidNode() const;
};

VIEW_END_NAMESPACE

#endif // RENDERENTRYMAPLIGHTPOSITIONAL_H
