/*! \file mainwin.h
 * \brief Defines a main application window corresponding to one currently open map document.
  */

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <QApplication>
#include <QCloseEvent>

class QMenu;
class QAction;
class QSize;

/**
 * @brief Application window class.
 *
 * Each MainWin correcponds to exactly one currently open map document. If a new map document is created, it requires a new MainWin.
 * When the last MainWin is closed, it will handle the closing of the log window if it is still open.
 */
class MainWin : public QMainWindow
{
  Q_OBJECT
public:
    /**
    * @brief Constructor.
    */
    MainWin();

    /**
    *  @brief Destructor.
    */
    ~MainWin() {}

    /**
     * @brief Returns the desired size for when the window is created.
     * @return Desired size.
     */
    QSize sizeHint() const { return QSize(800, 600); }
  
signals:
  
public slots:

protected:
    /**
     * @brief Called when this window is closed: handles checking of log window.
     * @param event Close event.
     */
    void closeEvent(QCloseEvent * event);

private:
    /**
     * @brief Creates menu bar on startup.
     */
    void CreateMenuBar();

    /**
     * @brief Creates menu actions on startup.
     */
    void CreateMenuActions();

    /**
     * @brief Sets up the status bar.
     */
    void SetUpStatusBar();

    /**
     * @brief Creates custom key shortcut sequences.
     */
    void CreateKeyShortcuts();

    QMenu*          m_pFileMenu;            /**< File menu. */
    QAction*        m_pActFileExit;         /**< Exit menu action. */

    QMenu*          m_pHelpMenu;            /**< Help menu. */
    QAction*        m_pActHelpAbout;        /**< About action. */

    QMenu*          m_pDeveloperMenu;       /**< Debug menu (shown only if debugging). */
    QAction*        m_pActShowConsole;      /**< Shows logging window. */
};

#endif // MAINWIN_H
