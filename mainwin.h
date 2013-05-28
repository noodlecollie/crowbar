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
    // Slot:    Updates the status bar's FPS meter with a float FPS value.
    // Input:   float   FPS value as a float.
    void UpdateFPSMeter(float);

    // Slot:    Resets FPS meter to "N/A".
    void ResetFPSMeter();

private:
    // Loads both static and non-static plugins.
    // Input:   QDir    Directory to load non-static plugins from.
    void LoadPlugins(QDir &);

    // Loads static plugins.
    void LoadStaticPlugins();

    // Loads non-static plugins from specified directory.
    // Input: QDir  Directory to load non-static plugins from.
    void LoadNonStaticPlugins(QDir &);

    // Creates menu bar on startup.
    void CreateMenuBar();

    // Creates menu actions on startup.
    void CreateMenuActions();

    // Sets up the status bar.
    void SetUpStatusBar();

    QList<QObject*> m_PluginList;           // List of currently loaded plugins.
    QList<QObject*> m_StaticPluginList;     // List of currently loaded static plugins.

    QMdiArea*       m_pMDI;                 // Central MDI area of window.

    QLabel*         m_pStatusFPS;           // FPS label for status bar.

    QMenu*          m_pFileMenu;            // File menu.
    QAction*        m_pActFileExit;         // Exit menu action.

    QMenu*          m_pHelpMenu;            // Help menu.
    QAction*        m_pActHelpAbout;        // About action.
};

#endif // MAINWIN_H
