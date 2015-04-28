#ifndef BRUSHADDWIDGET_H
#define BRUSHADDWIDGET_H

#include <QWidget>
#include <QVector3D>

namespace Ui {
class BrushAddWidget;
}

class BrushAddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrushAddWidget(QWidget *parent = 0);
    ~BrushAddWidget();

signals:
    void createBrush(QVector3D, QVector3D);

private slots:
    void create();

private:
    Ui::BrushAddWidget *ui;
};

#endif // BRUSHADDWIDGET_H
