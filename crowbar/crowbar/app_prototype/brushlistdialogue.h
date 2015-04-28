#ifndef BRUSHLISTDIALOGUE_H
#define BRUSHLISTDIALOGUE_H

#include <QDialog>
#include <QPointer>
#include "mapdocument.h"
#include "mapbrush.h"
#include <QList>
#include "mainwindow.h"

namespace Ui {
class BrushListDialogue;
}

class BrushListDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit BrushListDialogue(MODEL_NAMESPACE::MapDocument* document, MainWindow *parent = 0);
    ~BrushListDialogue();

private slots:
    void itemSelected();

private:
    Ui::BrushListDialogue *ui;
    QPointer<MODEL_NAMESPACE::MapDocument> m_pDocument;
    QList<QPointer<MODEL_NAMESPACE::MapBrush> > m_BrushList;
    QPointer<MODEL_NAMESPACE::MapBrush> m_pLastSelectedBrush;
};

#endif // BRUSHLISTDIALOGUE_H
