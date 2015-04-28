#include "brushaddwidget.h"
#include "ui_brushaddwidget.h"
#include <QtDebug>

BrushAddWidget::BrushAddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrushAddWidget)
{
    ui->setupUi(this);
}

BrushAddWidget::~BrushAddWidget()
{
    delete ui;
}

void BrushAddWidget::create()
{
    QVector3D min((float)qMin(ui->minXBox->value(), ui->maxXBox->value()),
                  (float)qMin(ui->minYBox->value(), ui->maxYBox->value()),
                  (float)qMin(ui->minZBox->value(), ui->maxZBox->value()));

    QVector3D max((float)qMax(ui->minXBox->value(), ui->maxXBox->value()),
                  (float)qMax(ui->minYBox->value(), ui->maxYBox->value()),
                  (float)qMax(ui->minZBox->value(), ui->maxZBox->value()));

    if ( min == max ) return;

    emit createBrush(min, max);
}
