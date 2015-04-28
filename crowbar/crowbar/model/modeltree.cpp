#include "modeltree.h"
#include "modeltreeitem.h"

MODEL_BEGIN_NAMESPACE

ModelTree::ModelTree(QObject *parent) :
    QObject(parent)
{
    m_iSize = 0;
    m_pRootItem = new ModelTreeItem();
    m_pRootItem->setParent(this);
    m_pRootItem->m_pParentTree = this;
}

ModelTree::~ModelTree()
{
    delete m_pRootItem;
}

int ModelTree::size() const
{
    return m_iSize;
}

void ModelTree::notifyItemAdded(ModelTreeItem *item)
{
    m_iSize++;
    emit itemAdded(item);
}

void ModelTree::notifyItemRemoved(ModelTreeItem *item)
{
    m_iSize--;
    emit itemRemoved(item);
}

ModelTreeItem* ModelTree::rootItem()
{
    return m_pRootItem;
}

const ModelTreeItem* ModelTree::rootItem() const
{
    return m_pRootItem;
}

MODEL_END_NAMESPACE
