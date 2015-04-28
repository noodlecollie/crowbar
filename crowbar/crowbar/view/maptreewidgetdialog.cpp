#include "maptreewidgetdialog.h"
#include "ui_maptreewidgetdialog.h"
#include <QPointer>
#include "mapcomponent.h"
#include "mapdocument.h"

using namespace MODEL_NAMESPACE;

VIEW_BEGIN_NAMESPACE

MapTreeWidgetDialog::MapTreeWidgetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapTreeWidgetDialog)
{
    ui->setupUi(this);
    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(handleCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

MapTreeWidgetDialog::~MapTreeWidgetDialog()
{
    delete ui;
}

MapTreeWidget* MapTreeWidgetDialog::treeWidget()
{
    return ui->treeWidget;
}

const MapTreeWidget* MapTreeWidgetDialog::treeWidget() const
{
    return ui->treeWidget;
}

void MapTreeWidgetDialog::handleCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if ( previous )
    {
        QPointer<MapComponent>* pointer = reinterpret_cast<QPointer<MapComponent>*>(previous->data(0, Qt::UserRole).value<qint64>());
        MapComponent* c = pointer->data();

        if ( c && !qobject_cast<MapWorld*>(c) )
        {
            MapDocument* doc = c->document();
            if ( doc )
            {
                doc->selectedSet().remove(c);
            }
        }
    }

    if ( current )
    {
        QPointer<MapComponent>* pointer = reinterpret_cast<QPointer<MapComponent>*>(current->data(0, Qt::UserRole).value<qint64>());
        MapComponent* c = pointer->data();

        if ( c && !qobject_cast<MapWorld*>(c) )
        {
            MapDocument* doc = c->document();
            if ( doc )
            {
                doc->selectedSet().clear();
                doc->selectedSet().add(c);
            }
        }
    }
}

VIEW_END_NAMESPACE
