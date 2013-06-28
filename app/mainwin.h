#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <QApplication>
#include <QCloseEvent>

#if defined Q_OS_WIN
#define PLUGIN_DIRECTORY QDir(qApp->applicationDirPath() + "\\plugins")

#elif defined Q_OS_MAC || defined Q_OS_LINUX
#define PLUGIN_DIRECTORY QDir(qApp->applicationDirPath() + "/plugins")

#endif

#define LABEL_FPS "FPS: "

class QMenu;
class QAction;
class QSize;

class MainWin : public QMainWindow
{
  Q_OBJECT
public:
  MainWin();
  ~MainWin();

  QSize sizeHint() const;
  
signals:
  
public slots:

protected:
  void closeEvent(QCloseEvent * event);

private:
    // Creates menu bar on startup.
    void CreateMenuBar();

    // Creates menu actions on startup.
    void CreateMenuActions();

    // Sets up the status bar.
    void SetUpStatusBar();

    // Creates custom key shortcut sequences.
    void CreateKeyShortcuts();

    QMenu*          m_pFileMenu;            // File menu.
    QAction*        m_pActFileExit;         // Exit menu action.

    QMenu*          m_pHelpMenu;            // Help menu.
    QAction*        m_pActHelpAbout;        // About action.

    QMenu*          m_pDebugMenu;           // Debug menu (shown only if debugging).
    QAction*        m_pActShowLogWindow;    // Shows logging window.
};

#endif // MAINWIN_H
