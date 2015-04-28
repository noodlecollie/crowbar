#include "brushlistdialogue.h"
#include "ui_brushlistdialogue.h"
#include <QList>

using namespace MODEL_NAMESPACE;

BrushListDialogue::BrushListDialogue(MapDocument *document, MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::BrushListDialogue)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_pDocument = document;
    if ( m_pDocument.isNull() ) return;

    for ( int i = 0; i < m_pDocument->brushes().count(); i++ )
    {
        m_BrushList.append(QPointer<MapBrush>(m_pDocument->brushes(i)));
        ui->listWidget->addItem(QString("%0").arg(i));
    }

    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelected()));
}

BrushListDialogue::~BrushListDialogue()
{
    if ( !m_pLastSelectedBrush.isNull() ) m_pLastSelectedBrush->setSelected(false);
    delete ui;
}

void BrushListDialogue::itemSelected()
{
    if ( m_pDocument.isNull() ) return;

    // Find out which item was selected.
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    QListWidgetItem* item = items.count() > 0 ? items.at(0) : NULL;

    if ( !item )
    {
        if ( !m_pLastSelectedBrush.isNull() )
        {
            m_pLastSelectedBrush->setSelected(false);
            m_pLastSelectedBrush.clear();
        }

        return;
    }

    // Get the brush for this item.
    QString indexStr = item->text();
    bool ok = false;
    int index = indexStr.toInt(&ok);
    if ( !ok )
    {
        if ( !m_pLastSelectedBrush.isNull() )
        {
            m_pLastSelectedBrush->setSelected(false);
            m_pLastSelectedBrush.clear();
        }
    }

    MapBrush* brush = m_pDocument->brushes(index);

    // If the brushes are not the same, update our selected state.
    if ( brush != m_pLastSelectedBrush.data() )
    {
        if ( !m_pLastSelectedBrush.isNull() )
        {
           m_pLastSelectedBrush->setSelected(false);
        }

        if ( brush )
        {
            brush->setSelected(true);
        }

        m_pLastSelectedBrush = brush;
        MainWindow* mainwin = qobject_cast<MainWindow*>(parent());
        if ( mainwin )
        {
            mainwin->notifySelectedSetUpdated();
            mainwin->updateViews();
        }
    }
}
