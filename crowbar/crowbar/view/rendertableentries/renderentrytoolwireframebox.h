#ifndef RENDERENTRYTOOLWIREFRAMEBOX_H
#define RENDERENTRYTOOLWIREFRAMEBOX_H

#include "rendertableentry.h"
#include "toolwireframebox.h"

class QGLAbstractEffect;

VIEW_BEGIN_NAMESPACE

class RenderEntryToolWireframeBox : public RenderTableEntry
{
    Q_OBJECT
public:
    RenderEntryToolWireframeBox(MODEL_NAMESPACE::ToolWireframeBox* component, MapDocumentView* parent = 0);
    virtual ~RenderEntryToolWireframeBox();

    inline MODEL_NAMESPACE::ToolWireframeBox* box() const
    {
        return qobject_cast<MODEL_NAMESPACE::ToolWireframeBox*>(component());
    }

    virtual bool isValid() const { return true; }

protected:
    virtual QGLSceneNode* sceneNodeFor(IRender::RenderMode mode) const;

private:
    QGLSceneNode* buildNode(IRender::RenderMode mode) const;
};

VIEW_END_NAMESPACE

#endif // RENDERENTRYTOOLWIREFRAMEBOX_H
