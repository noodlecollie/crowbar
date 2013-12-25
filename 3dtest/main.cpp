#include <QApplication>
 #include "teapotview.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     TeapotView view;

 #ifdef Q_OS_SYMBIAN
     view.setAttribute(Qt::WA_LockLandscapeOrientation, true);
     view.showFullScreen();
 #else
     if (view.stereoType() != QGLView::RedCyanAnaglyph)
         view.camera()->setEyeSeparation(0.3f);
     if (QApplication::arguments().contains(QLatin1String("-maximize")))
         view.showMaximized();
     else if (QApplication::arguments().contains(QLatin1String("-fullscreen")))
         view.showFullScreen();
     else
         view.show();
 #endif

     return app.exec();
 }
