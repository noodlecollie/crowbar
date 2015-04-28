#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openglwidgetv2.h"
#include "brushaddwidget.h"
#include "mapdocument.h"

class QDockWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void createBrush(const QVector3D &min, const QVector3D &max);

private:
    OpenGLWidgetV2* m_pView;
    QDockWidget*    m_pDockAddWidget;
    BrushAddWidget* m_pAddWidget;
    MODEL_NAMESPACE::MapDocument*    m_pDoc;
};

#endif // MAINWINDOW_H
