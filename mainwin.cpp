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

  // Set up the main window structure.
  m_pMDI = new QMdiArea(this);
  this->setCentralWidget(m_pMDI);

  // Create menu actions and menu bar.
  CreateMenuActions();
  CreateMenuBar();

  // Set up status bar.
  SetUpStatusBar();

  // Load plugins.
  LoadPlugins(PLUGIN_DIRECTORY);

  // Set status bar text.
  ResetFPSMeter();
}

// Loads plugins from the specified directory.
// Input: QDir directory  Directory to load from.
void MainWin::LoadPlugins(QDir &directory)
{
  // Load static plugins first.
  LoadStaticPlugins();

  // Then dynamic plugins.
  LoadNonStaticPlugins(directory);
}

// Loads static plugins.
void MainWin::LoadStaticPlugins()
{
  // StaticInstances() will give us all the static plugins to load.
  foreach (QObject *plugin, QPluginLoader::staticInstances())
  {
    if ( plugin ) m_StaticPluginList.append(plugin);  // Add the plugin pointer to our list.
  }
}

// Loads non-static plugins from specified directory.
// Input: QDir  Directory to load non-static plugins from.
void MainWin::LoadNonStaticPlugins(QDir &directory)
{
  // Iterate through each file in the directory.
  foreach (QString fileName, directory.entryList(QDir::Files))
  {
    QPluginLoader loader(directory.absoluteFilePath(fileName));  // Create a loader for the file using the absolute path.
    QObject *plugin = loader.instance();                          // Attempt to instance (ie. load) the plugin.

    // If successful:
    if ( plugin )
    {
      m_PluginList.append(plugin);  // Add to list.
                                        // Note that plugins are automatically unloaded on application exit.
    }
  }
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

    // Add the FPS label.
    m_pStatusFPS = new QLabel();
    statusBar()->addPermanentWidget(m_pStatusFPS);  // Auto-parented.
}

// Slot:    Updates the status bar's FPS meter with a float FPS value.
// Input:   float fps   FPS value as a float.
void MainWin::UpdateFPSMeter(float fps)
{
    // Make sure the FPS label is valid.
    if ( !m_pStatusFPS ) return;

    // Set the label.
    QString txt(LABEL_FPS);
    txt += QString::number(fps);
    m_pStatusFPS->setText(txt);
}

// Slot:    Resets FPS meter to "N/A".
void MainWin::ResetFPSMeter()
{
    QString txt(LABEL_FPS);
    txt += "N/A";
    m_pStatusFPS->setText(txt);
}
