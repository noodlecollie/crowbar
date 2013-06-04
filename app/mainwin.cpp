#include "mainwin.h"
#include <QMessageBox>
#include <QPluginLoader>
#include <QMenuBar>
#include <QLabel>
#include <QMdiArea>
#include <QDir>
#include <QStatusBar>

MainWin::MainWin()
{
    setMinimumSize(320, 240);

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
    connect(m_pActFileExit, SIGNAL(triggered()), this, SLOT(close()));

    m_pActHelpAbout = new QAction(tr("About &Qt"), this);
    connect(m_pActHelpAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

// Creates menu bar on startup.
void MainWin::CreateMenuBar()
{
    // File menu
    m_pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pFileMenu->addAction(m_pActFileExit);

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
