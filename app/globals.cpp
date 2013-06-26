#include "logwindow.h"
#include "commandlineparser.h"
#include <QMessageBox>

LogWindow*          g_pLog;
CommandLineParser*  g_pCmdLine;

// Creates a simple QMessageBox with an OK button that shows the specified message.
void ShowMessageBox(QString message)
{
    QMessageBox box;
    box.setMinimumSize(300, 200);
    box.setText(message);
    box.exec();
}
