#include <QString>
#include <QtTest>
#include "modeltree.h"
#include "modeltreeitem.h"
#include <QHash>

using namespace MODEL_NAMESPACE;

class CustomSignalChecker : public QObject
{
    Q_OBJECT
public:
    explicit CustomSignalChecker(QObject* parent = 0) : QObject(parent)
    {
        m_iItems = 0;
    }

    int itemNumber(ModelTreeItem* item) const
    {
        return m_ItemTable.value(item, -1);
    }

    int itemsStored() const
    {
        return m_ItemTable.count();
    }

    QList<int> removalOrder() const
    {
        return m_RemovalOrder;
    }

public slots:
    void addItem(ModelTreeItem* item)
    {
        m_ItemTable.insert(item, m_iItems);
        m_iItems++;
    }

    void removeItem(ModelTreeItem* item)
    {
        int i = m_ItemTable.value(item, -1);
        if ( i > 0 ) m_RemovalOrder.append(i);
        m_ItemTable.remove(item);
    }

private:
    int m_iItems;
    QHash<ModelTreeItem*, int> m_ItemTable;
    QList<int> m_RemovalOrder;
};

class tst_ModelTree : public QObject
{
    Q_OBJECT

public:
    tst_ModelTree()
    {
    }

private Q_SLOTS:
    void zeroItemsOnConstruction()
    {
        ModelTree tree;
        QVERIFY2(tree.size() == 0, "Tree size not zero on construction.");
    }

    void rootNodePresentOnConstruction()
    {
        ModelTree tree;
        QVERIFY2(tree.rootItem(), "Root item is NULL.");
    }

    void addOneItemToTree()
    {
        ModelTree tree;
        ModelTreeItem* item = new ModelTreeItem();
        item->setParentItem(tree.rootItem());
        QVERIFY2(tree.size() == 1, "Item not correctly added to tree.");
    }

    void addOneItemToTreeWithConstructor()
    {
        ModelTree tree;
        ModelTreeItem* item = new ModelTreeItem(tree.rootItem());
        Q_UNUSED(item);
        QVERIFY2(tree.size() == 1, "Item not correctly added to tree.");
    }

    void removeOneItemFromTree()
    {
        ModelTree tree;

        ModelTreeItem* latestItem = NULL;
        for ( int i = 0; i < 5; i++ )
        {
            latestItem = new ModelTreeItem(tree.rootItem());
        }

        delete latestItem;
        QVERIFY2(tree.size() == 4, "Item not removed from tree correctly.");
    }

    void addSubtreeToTree()
    {
        ModelTree tree;

        ModelTreeItem* item1 = new ModelTreeItem();
        ModelTreeItem* item2 = new ModelTreeItem(item1);
        ModelTreeItem* item3 = new ModelTreeItem(item1);
        ModelTreeItem* item4 = new ModelTreeItem(item2);
        Q_UNUSED(item4);
        Q_UNUSED(item3);

        item1->setParentItem(tree.rootItem());

        QVERIFY2(tree.size() == 4, "Subtree of items not added to tree correctly.");
    }

    void removeSubtreeFromTree()
    {
        ModelTree tree;

        ModelTreeItem* item1 = new ModelTreeItem(tree.rootItem());
        ModelTreeItem* item2 = new ModelTreeItem(item1);
        ModelTreeItem* item3 = new ModelTreeItem(item2);
        ModelTreeItem* item4 = new ModelTreeItem(item3);
        Q_UNUSED(item4);

        delete item2;

        QVERIFY2(tree.size() == 1, "Subtree of items not removed from tree correctly.");
    }

    void checkItemParentTreeOnAdd()
    {
        ModelTree tree;
        ModelTreeItem* item = new ModelTreeItem();
        item->setParentItem(tree.rootItem());
        QVERIFY2(item->parentTree() == &tree, "Item parent tree not set on tree add.");
    }

    void checkItemParentItemOnAdd()
    {
        ModelTree tree;
        ModelTreeItem* item = new ModelTreeItem();
        item->setParentItem(tree.rootItem());
        QVERIFY2(item->parentItem() == tree.rootItem(), "Item parent not set on tree add.");
    }

    void checkItemParentTreeOnConstructorAdd()
    {
        ModelTree tree;
        ModelTreeItem* item = new ModelTreeItem(tree.rootItem());
        QVERIFY2(item->parentTree() == &tree, "Item parent tree not set on tree add from item constructor.");
    }

    void checkItemParentItemOnConstructorAdd()
    {
        ModelTree tree;
        ModelTreeItem* item = new ModelTreeItem(tree.rootItem());
        QVERIFY2(item->parentItem() == tree.rootItem(), "Item parent not set on tree add from item constructor.");
    }

    void checkItemParentsOnSubtreeAdd()
    {
        ModelTree tree;

        ModelTreeItem* item1 = new ModelTreeItem();
        ModelTreeItem* item2 = new ModelTreeItem(item1);
        ModelTreeItem* item3 = new ModelTreeItem(item1);
        ModelTreeItem* item4 = new ModelTreeItem(item2);
        Q_UNUSED(item4);
        Q_UNUSED(item3);

        item1->setParentItem(tree.rootItem());

        QVERIFY2(item1->parentTree() == &tree &&
                 item2->parentTree() == &tree &&
                 item3->parentTree() == &tree &&
                 item4->parentTree() == &tree,
                 "Child items' parent trees are not set correctly when adding an item subtree.");
    }

    void checkItemsRemovedWhenTreeDestroyed()
    {
        ModelTree* tree = new ModelTree();
        CustomSignalChecker rcv;
        connect(tree, SIGNAL(itemAdded(ModelTreeItem*)), &rcv, SLOT(addItem(ModelTreeItem*)));
        connect(tree, SIGNAL(itemRemoved(ModelTreeItem*)), &rcv, SLOT(removeItem(ModelTreeItem*)));

        // Level 1 items
        ModelTreeItem* l1_1 = new ModelTreeItem(tree->rootItem());
        ModelTreeItem* l1_2 = new ModelTreeItem(tree->rootItem());
        ModelTreeItem* l1_3 = new ModelTreeItem(tree->rootItem());
        Q_UNUSED(l1_2);
        Q_UNUSED(l1_3);

        // Level 2 items
        ModelTreeItem* l2_1 = new ModelTreeItem(l1_1);
        ModelTreeItem* l2_2 = new ModelTreeItem(l1_1);
        Q_UNUSED(l2_1);
        Q_UNUSED(l2_2);

        delete tree;
        tree = NULL;

        QVERIFY2(rcv.itemsStored() == 0, "Items not removed from tree correctly on destruction.");
    }

    void checkItemsRemovedInOrderWhenTreeDestroyed()
    {
        ModelTree* tree = new ModelTree();
        CustomSignalChecker rcv;
        connect(tree, SIGNAL(itemAdded(ModelTreeItem*)), &rcv, SLOT(addItem(ModelTreeItem*)));
        connect(tree, SIGNAL(itemRemoved(ModelTreeItem*)), &rcv, SLOT(removeItem(ModelTreeItem*)));

        // Level 1 items
        ModelTreeItem* l1_1 = new ModelTreeItem(tree->rootItem());  // 0
        ModelTreeItem* l1_2 = new ModelTreeItem(tree->rootItem());  // 1
        ModelTreeItem* l1_3 = new ModelTreeItem(tree->rootItem());  // 2
        Q_UNUSED(l1_3);

        // Level 2 items
        ModelTreeItem* l2_1 = new ModelTreeItem(l1_1);  // 3
        ModelTreeItem* l2_2 = new ModelTreeItem(l1_1);  // 4
        ModelTreeItem* l2_3 = new ModelTreeItem(l1_2);  // 5
        Q_UNUSED(l2_2);

        // Level 3 items
        ModelTreeItem* l3_1 = new ModelTreeItem(l2_1);  // 6
        ModelTreeItem* l3_2 = new ModelTreeItem(l2_3);  // 7
        Q_UNUSED(l3_1);
        Q_UNUSED(l3_2);

        delete tree;
        tree = NULL;

        QList<int> ordr = rcv.removalOrder();

        QVERIFY2(ordr.indexOf(6) < ordr.indexOf(3) &&
                 ordr.indexOf(7) < ordr.indexOf(5),
                 "Level 2 and 3 items not removed in correct order");

        QVERIFY2(ordr.indexOf(3) < ordr.indexOf(0) &&
                 ordr.indexOf(4) < ordr.indexOf(0) &&
                 ordr.indexOf(5) < ordr.indexOf(1),
                 "Level 1 and 2 items not removed in correct order.");
    }
};

QTEST_APPLESS_MAIN(tst_ModelTree)

#include "tst_tst_modeltree.moc"
