#include "mainwin.h"
#include <QMessageBox>
#include <QPluginLoader>
#include <QMenuBar>
#include <QLabel>
#include <QMdiArea>
#include <QDir>
#include <QStatusBar>
#include <QApplication>
#include <QWindowList>
#include <QWindow>
#include "globals.h"
#include "commandlineparser.h"
#include "logwindow.h"
#include <QList>
#include <QSize>

MainWin::MainWin()
{
    // Add to window tracker.
    if ( !g_pWindowTracker->contains(this) ) g_pWindowTracker->append(this);

    setMinimumSize(320, 240);   // We don't want to go smaller than this.

    // Create menu actions and menu bar.
    CreateMenuActions();
    CreateMenuBar();

    // Set up status bar.
    SetUpStatusBar();
}

// Creates menu actions on startup.
void MainWin::CreateMenuActions()
{
    // Set exit action to close the main window.
    m_pActFileExit = new QAction(tr("E&xit"), this);
    m_pActFileExit->setShortcuts(QKeySequence::Quit);
    connect(m_pActFileExit, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Set "about" action to bring up the "about" window.
    m_pActHelpAbout = new QAction(tr("About &Qt"), this);
    connect(m_pActHelpAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // Set log window action to bring up the log window.
    m_pActShowConsole = new QAction(tr("&Console"), this);
    //m_pActShowLogWindow->setShortcuts(*m_keyShowLogWindow);   // TODO: Get custom shortcut working for this (shift-tab?)
    connect(m_pActShowConsole, SIGNAL(triggered()), g_pLog, SLOT(showAndRaise()));
}

// Creates menu bar on startup.
void MainWin::CreateMenuBar()
{
    // File menu
    m_pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pFileMenu->addAction(m_pActFileExit);

    // Debug menu
    m_pDeveloperMenu = menuBar()->addMenu(tr("&Developer"));
    m_pDeveloperMenu->addAction(m_pActShowConsole);

    // Help menu
    m_pHelpMenu = menuBar()->addMenu(tr("&Help"));
    m_pHelpMenu->addAction(m_pActHelpAbout);
}

// Sets up status bar.
void MainWin::SetUpStatusBar()
{
    // The status bar is automatically created by the first call to statusBar().
    statusBar()->setSizeGripEnabled(true);
}

void MainWin::closeEvent(QCloseEvent * event)
{
    QWidget::closeEvent(event); // Handle close event first.

    // Remove from window tracker.
    g_pWindowTracker->removeAt(g_pWindowTracker->indexOf(this));

    // If there are no windows left in the tracker, quit.
    if ( g_pWindowTracker->size() < 1 ) qApp->quit();
}
