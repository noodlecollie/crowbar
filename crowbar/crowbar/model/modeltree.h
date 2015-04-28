#ifndef MODELTREE_H
#define MODELTREE_H

#include "model_global.h"
#include <QObject>

MODEL_BEGIN_NAMESPACE

class ModelTreeItem;

class MODELSHARED_EXPORT ModelTree : public QObject
{
    Q_OBJECT
    friend class ModelTreeItem;

    Q_PROPERTY(int size READ size)
public:
    explicit ModelTree(QObject *parent = 0);
    ~ModelTree();

    // Does not include the root item.
    int size() const;

    ModelTreeItem* rootItem();
    const ModelTreeItem* rootItem() const;

signals:
    void itemAdded(ModelTreeItem*);
    void itemRemoved(ModelTreeItem*);

public slots:

private slots:
    void notifyItemAdded(ModelTreeItem* item);
    void notifyItemRemoved(ModelTreeItem* item);

private:
    ModelTreeItem*  m_pRootItem;
    int             m_iSize;
};

MODEL_END_NAMESPACE

#endif // MODELTREE_H
