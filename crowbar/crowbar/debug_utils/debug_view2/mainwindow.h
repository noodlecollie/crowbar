#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapview2d.h"
#include "mapview3d.h"

MODEL_BEGIN_NAMESPACE
class MapDocument;
MODEL_END_NAMESPACE

VIEW_BEGIN_NAMESPACE
class MapDocumentRenderer2;
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
    void setDrawGrid(bool enabled);

    void setZPos();
    void setZNeg();
    void setYPos();
    void setYNeg();
    void setXPos();
    void setXNeg();

private:
    Ui::MainWindow *ui;
    VIEW_NAMESPACE::MapView2D*              m_pView;
    VIEW_NAMESPACE::MapView3D*              m_p3DView;
    MODEL_NAMESPACE::MapDocument*           m_pDocument;
    VIEW_NAMESPACE::MapDocumentRenderer2*   m_pRenderer;
};

#endif // MAINWINDOW_H
