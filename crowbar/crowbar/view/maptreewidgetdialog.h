#ifndef MAPTREEWIDGETDIALOG_H
#define MAPTREEWIDGETDIALOG_H

#include <QDialog>
#include "view_global.h"
#include "maptreewidget.h"

namespace Ui {
class MapTreeWidgetDialog;
}

VIEW_BEGIN_NAMESPACE

class VIEWSHARED_EXPORT MapTreeWidgetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapTreeWidgetDialog(QWidget *parent = 0);
    ~MapTreeWidgetDialog();

    MapTreeWidget* treeWidget();
    const MapTreeWidget* treeWidget() const;

private slots:
    void handleCurrentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

private:
    Ui::MapTreeWidgetDialog *ui;
};

VIEW_END_NAMESPACE

#endif // MAPTREEWIDGETDIALOG_H
