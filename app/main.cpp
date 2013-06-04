#include <QApplication>

#include "mainwin.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWin win;
    win.show();

    return app.exec();  // exec() starts the event loop. No user interaction should occur before this, but computation is fine.
}
