#ifndef TEAPOTVIEW_H
 #define TEAPOTVIEW_H

 #include "qglview.h"
 #include "qglteapot.h"

 class QGLSceneNode;

 class TeapotView : public QGLView
 {
     Q_OBJECT
 public:
     TeapotView(QWidget *parent = 0) : QGLView(parent), teapot(0) {}
     ~TeapotView();

 protected:
     void initializeGL(QGLPainter *painter);
     void paintGL(QGLPainter *painter);

 private:
     QGLSceneNode *teapot;
 };

 #endif
