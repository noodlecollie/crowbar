#ifndef RENDERENTRYTOOLBASICSQUARE_H
#define RENDERENTRYTOOLBASICSQUARE_H

#include "rendertableentry.h"
#include "model_global.h"
#include "toolbasicsquare.h"
#include <QGeometryData>

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT RenderEntryToolBasicSquare : public RenderTableEntry
{
    Q_OBJECT
public:
    RenderEntryToolBasicSquare(MODEL_NAMESPACE::ToolBasicSquare* component, MapDocumentView* parent = 0);
    virtual ~RenderEntryToolBasicSquare();

    inline MODEL_NAMESPACE::ToolBasicSquare* square() const
    {
        return qobject_cast<MODEL_NAMESPACE::ToolBasicSquare*>(component());
    }

    virtual bool isValid() const { return true; }

signals:

public slots:

protected:
    virtual QGLSceneNode* sceneNodeFor(IRender::RenderMode mode) const;

private:
    QGLSceneNode* buildSceneNode() const;
    QGeometryData innerFace() const;
    QGeometryData edges() const;
};

VIEW_END_NAMESPACE

#endif // RENDERENTRYTOOLBASICSQUARE_H
