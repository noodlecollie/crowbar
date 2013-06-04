#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <QApplication>

#if defined Q_OS_WIN
#define PLUGIN_DIRECTORY QDir(qApp->applicationDirPath() + "\\plugins")

#elif defined Q_OS_MAC || defined Q_OS_LINUX
#define PLUGIN_DIRECTORY QDir(qApp->applicationDirPath() + "/plugins")

#endif

#define LABEL_FPS "FPS: "

class QDir;
class QMdiArea;
class QMenu;
class QLabel;

class MainWin : public QMainWindow
{
  Q_OBJECT
public:
  MainWin();
  
signals:
  
public slots:

private:
    // Creates menu bar on startup.
    void CreateMenuBar();

    // Creates menu actions on startup.
    void CreateMenuActions();

    // Sets up the status bar.
    void SetUpStatusBar();

    QMenu*          m_pFileMenu;            // File menu.
    QAction*        m_pActFileExit;         // Exit menu action.

    QMenu*          m_pHelpMenu;            // Help menu.
    QAction*        m_pActHelpAbout;        // About action.
};

#endif // MAINWIN_H
