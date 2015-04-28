#ifndef ORTHOGRAPHICSUBWINDOW_H
#define ORTHOGRAPHICSUBWINDOW_H

#include <QWidget>
#include "view_global.h"

VIEW_BEGIN_NAMESPACE
class MapView2D;
VIEW_END_NAMESPACE

namespace Ui {
class OrthographicSubWindow;
}

class OrthographicSubWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit OrthographicSubWindow(QWidget *parent = 0);
    ~OrthographicSubWindow();
    
    VIEW_NAMESPACE::MapView2D* view();
    const VIEW_NAMESPACE::MapView2D* view() const;
    
signals:
    
private slots:
    void setTop();
    void setFront();
    void setSide();
    
private:
    Ui::OrthographicSubWindow *ui;
};

#endif // ORTHOGRAPHICSUBWINDOW_H
