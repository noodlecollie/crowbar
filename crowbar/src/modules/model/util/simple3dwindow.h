#ifndef SIMPLE3DWINDOW_H
#define SIMPLE3DWINDOW_H

#include "model_global.h"
#include <QWindow>
#include <QOpenGLFunctions>

namespace Qt3D
{
class QAspectEngine;
class QRenderAspect;
class QInputAspect;
class QEntity;
class QCamera;
class QFrameGraph;
class QForwardRenderer;
}

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT Simple3DWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Simple3DWindow(QScreen *screen = 0);
    ~Simple3DWindow();

    void enableRootEntity();
    Qt3D::QEntity* rootEntity() const;

signals:

public slots:

protected:
    virtual void initLights();
    virtual void initCamera();
    virtual void initNodes();
    virtual void initRenderer();

private:
    void initialise();
    void shutdown();

    Qt3D::QAspectEngine*    m_pAspectEngine;
    Qt3D::QRenderAspect*    m_pRenderAspect;
    Qt3D::QInputAspect*     m_pInputAspect;
    Qt3D::QEntity*          m_pRootEntity;
    Qt3D::QCamera*          m_pCamera;
    Qt3D::QFrameGraph*      m_pFrameGraph;
    Qt3D::QForwardRenderer* m_pForwardRenderer;
};

MODEL_END_NAMESPACE

#endif // SIMPLE3DWINDOW_H
