#include "teapotview.h"
#include "qglbuilder.h"
#include "qglscenenode.h"

void TeapotView::initializeGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    QGLBuilder builder;
    builder << QGLTeapot();
    teapot = builder.finalizedSceneNode();
}

TeapotView::~TeapotView()
{
    delete teapot;
}

void TeapotView::paintGL(QGLPainter *painter)
{
    teapot->draw(painter);
}
