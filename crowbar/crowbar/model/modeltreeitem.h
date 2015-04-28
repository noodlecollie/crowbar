#ifndef MODELTREEITEM_H
#define MODELTREEITEM_H

#include <QObject>
#include "model_global.h"

MODEL_BEGIN_NAMESPACE

class ModelTree;

class MODELSHARED_EXPORT ModelTreeItem : public QObject
{
    Q_OBJECT
    friend class ModelTree;
    friend class ModelTreeItem;
public:
    explicit ModelTreeItem(ModelTreeItem *parent = 0);
    ~ModelTreeItem();

    inline ModelTreeItem* parentItem() const
    {
        return qobject_cast<ModelTreeItem*>(parent());
    }
    void setParentItem(ModelTreeItem* item);

    ModelTree* parentTree() const;

    inline QList<ModelTreeItem*> childItems() const
    {
        return findChildren<ModelTreeItem*>(QString(), Qt::FindDirectChildrenOnly);
    }

signals:

public slots:

private:
    void setParentTree(ModelTree* tree);

    ModelTree*  m_pParentTree;
};

MODEL_END_NAMESPACE

#endif // MODELTREEITEM_H
