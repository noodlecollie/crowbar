#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapdocument.h"
#include "inputtranslator.h"
#include "orthographicsubwindow.h"
#include <QSplitter>
#include "texturetabledockwidget.h"
#include <QVBoxLayout>
#include "maptreewidgetdialog.h"

VIEW_BEGIN_NAMESPACE
class MapView;
class MapView3D;
VIEW_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void toggleSecondaryView();
    
private slots:
    void doCreateDocument();
    void handleDocumentCreated(MODEL_NAMESPACE::MapDocument*);
    void handleDocumentRemoved(MODEL_NAMESPACE::MapDocument*);
    void chooseDocument(QAction* action);
    void closeCurrentDocument();
    void executeCurrentTool();

    void increaseCurrentGridPower();
    void decreaseCurrentGridPower();
    void enableCurrentToolSnapping();
    void disableCurrentToolSnapping();

    void toolChosen();
    void viewFocusGained(VIEW_NAMESPACE::MapView* view);
    void viewFocusLost(VIEW_NAMESPACE::MapView* view);

    void saveCurrentDocumentAs();
    void changeCurrentTexture(const QString &name);

    void showTreeView();
    
protected:
    void closeEvent(QCloseEvent *);

private:
    void updateDocumentMenuList();
    void setCurrentDocument(int index);
    void setUpInputTranslator();
    void handleCurrentDocumentChange(MODEL_NAMESPACE::MapDocument* vOld, MODEL_NAMESPACE::MapDocument* vNew);
    void updateStatusBar();
    MODEL_NAMESPACE::MapDocument* currentDocument() const;
    
    Ui::MainWindow *ui;
    int m_iCurrentDocumentIndex;
    int m_iCurrentToolIndex;
    MODEL_NAMESPACE::InputTranslator* m_pInputTranslator;
    OrthographicSubWindow*  m_pSubWindow;
    VIEW_NAMESPACE::MapView3D*  m_p3DView;
    QSplitter* m_pSplitter;
    TextureTableDockWidget* m_pTexDockWidget;
    QWidget* m_pPlaceholderWidget;
    QVBoxLayout* m_pPlaceholderLayout;
    VIEW_NAMESPACE::MapTreeWidgetDialog* m_pTreeWidget;
};

#endif // MAINWINDOW_H
