#ifndef IRENDER_H
#define IRENDER_H

#include "view_global.h"
#include <QSize>
#include "mapcomponent.h"
#include "mapface.h"
#include <QRect>
#include <QBox3D>

class QGLPainter;
class QGLLightParameters;
class QGLLightModel;

VIEW_BEGIN_NAMESPACE

class IRender
{
public:
    enum RenderMode
    {
        Triangles = 0,
        Edges,
        
        TOTAL_RENDER_MODES,
    };

    virtual ~IRender() {}

    virtual void draw(QGLPainter* painter, RenderMode mode, const QSize &viewSize) = 0;

    virtual QList<MODEL_NAMESPACE::MapComponent*>
    objectsWithin(const QRect &, QGLPainter*, RenderMode, const QSize &)
    {
        return QList<MODEL_NAMESPACE::MapComponent*>();
    }

    virtual MODEL_NAMESPACE::MapFace* faceAt(const QPoint &, QGLPainter*, RenderMode, const QSize&)
    {
        return NULL;
    }

    virtual bool supportsObjectPicking() const
    {
        return false;
    }
    
    virtual QList<const QGLLightParameters*> lights() const
    {
        return QList<const QGLLightParameters*>();
    }

    virtual QGLLightModel* lightModel()
    {
        return NULL;
    }

    virtual const QGLLightModel* lightModel() const
    {
        return NULL;
    }

    virtual QBox3D selectionBounds() const
    {
        return QBox3D();
    }
};

VIEW_END_NAMESPACE

#endif // IRENDER_H
