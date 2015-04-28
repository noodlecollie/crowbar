#include "modeltreeitem.h"
#include <QtDebug>
#include "modeltree.h"

MODEL_BEGIN_NAMESPACE

ModelTreeItem::ModelTreeItem(ModelTreeItem *parent) :
    QObject(NULL)
{
    setParentItem(parent);
}

ModelTreeItem::~ModelTreeItem()
{
    // Delete children first.
    QList<ModelTreeItem*> list = childItems();
    foreach ( ModelTreeItem* item, list )
    {
        delete item;
    }

    // Now that our children are deleted, delete us too.
    // Notify the parent tree.
    if ( parentTree() ) parentTree()->notifyItemRemoved(this);
}

ModelTree* ModelTreeItem::parentTree() const
{
    return m_pParentTree;
}

void ModelTreeItem::setParentItem(ModelTreeItem *item)
{
    Q_ASSERT(!parentTree() || parentTree() == item->parentTree());
    if ( parentTree() )
    {
        if ( parentTree() != item->parentTree() )
        {
            qWarning() << "Parenting to a different ModelTree is not allowed!";
            return;
        }
        else if ( !item )
        {
            qWarning() << "Unparenting from the parent ModelTree is not allowed!";
            return;
        }
    }

    setParent(item);
    setParentTree(item ? item->parentTree() : NULL);
}

void ModelTreeItem::setParentTree(ModelTree *tree)
{
    if ( m_pParentTree && tree != m_pParentTree )
    {
        qWarning() << "Parenting to a different ModelTree is not allowed!";
        return;
    }

    if ( tree == m_pParentTree ) return;

    // If we get here, we're gaining a ModelTree from NULL.
    m_pParentTree = tree;

    // Notify the tree than we're adding to it.
    m_pParentTree->notifyItemAdded(this);

    // Update the parent tree for any children.
    QList<ModelTreeItem*> list = childItems();
    foreach ( ModelTreeItem* item, list )
    {
        item->setParentTree(m_pParentTree);
    }
}

MODEL_END_NAMESPACE
