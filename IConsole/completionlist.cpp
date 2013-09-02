#include <QKeyEvent>
#include <QScrollBar>
#include "completionlist.h"

CompletionList::CompletionList(QWidget *parent) :
    QListWidget(parent)
{
    setUniformItemSizes(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void CompletionList::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Return) emit itemActivated(currentItem());
    else QListWidget::keyPressEvent(e);
}

QSize CompletionList::sizeHint() const
{
    QAbstractItemModel *model = this->model();
    QAbstractItemDelegate *delegate = this->itemDelegate();
    const QStyleOptionViewItem sovi;
    int left, top, right, bottom = 0;
    getContentsMargins(&left, &top, &right, &bottom);
    const int vOffset = top + bottom;
    const int hOffset = left + right;
    
    bool vScrollOn = false;
    int height = 0;
    int width = 0;
    for (int i = 0; i < count(); i++)
    {
        QModelIndex index = model->index(i, 0);
        QSize itemSizeHint = delegate->sizeHint(sovi, index);
        if (itemSizeHint.width() > width)
                width = itemSizeHint.width();

        // height
        const int nextHeight = height + itemSizeHint.height();
        if (nextHeight + vOffset < maximumHeight()) height = nextHeight;
        else
        {
            // early termination
            vScrollOn = true;
            break;
        }
    }

    QSize sizeHint(width + hOffset, 0);
    sizeHint.setHeight(height + vOffset);
    if (vScrollOn)
    {
        int scrollWidth = this->verticalScrollBar()->sizeHint().width();
        sizeHint.setWidth(sizeHint.width() + scrollWidth);
    }
    
    return sizeHint;
}
