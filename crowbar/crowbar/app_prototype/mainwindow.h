#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include "mapview2d.h"
#include "mapview3d.h"
#include "mapdocument.h"
#include <QPointer>
#include "basetool.h"
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
signals:
    void selectedSetUpdated();

public slots:
    void createNewDocument();
    void updateViews();
    void notifySelectedSetUpdated() { emit selectedSetUpdated(); }

    void setTopView();
    void setFrontView();
    void setSideView();

    void loadTextures();

private slots:
    void updateDocumentMenu();
    void handleDocumentMenuItemChosen();
    void showBrushDialogue();
    void showTransformDialogue();

private:
    void currentDocumentUpdated();

    Ui::MainWindow *ui;

    QSplitter*  m_pSplitter;
    VIEW_NAMESPACE::MapView2D*  m_p2DView;
    VIEW_NAMESPACE::MapView3D*  m_p3DView;
    QPointer<MODEL_NAMESPACE::MapDocument> m_pCurrentDocument;
    QPointer<TOOLS_NAMESPACE::BaseTool> m_pCurrentTool;
    QString m_szTextureDir;
};

#endif // MAINWINDOW_H
