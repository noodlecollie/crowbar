#include "modeltreea.h"
#include <QMetaProperty>

ModelTreeA::ModelTreeA(QObject *parent) : QObject(parent)
{
    m_pRootItem = new QObject(this);
}

ModelTreeA::~ModelTreeA()
{
    // Everything will be deleted when the root item is deleted.
}

QObject* ModelTreeA::getItem(const QModelIndex &index) const
{
    if ( index.isValid() )
    {
        QObject *item = static_cast<QObject*>(index.internalPointer());
        if ( item ) return item;
    }

    // Invalid indices, or indices that can't be cast to QObject, return the root item instead.
    return m_pRootItem;
}

bool ModelTreeA::isValidForParentIndex(const QObject *obj, int row)
{
    row = rowToChildIndex(obj, row);
    return row > 0 && row < obj->children().count();
}

int ModelTreeA::rowToChildIndex(const QObject *parent, int row)
{
    return row - parent->metaObject()->propertyCount();
}

QModelIndex ModelTreeA::index(int row, int column, const QModelIndex &parent) const
{
    if ( column < 0 || column > 1 || row < 0 ) return QModelIndex();

    // The index is constructed as follows:
    // - The internal pointer points to the raw QObject that holds this particular property or child.
    // - If the supplied parent index is valid but not for column 1, or isValidForParentIndex() returns false given
    //   the row and pointer to the parent object, an invalid index should be returned.

    if ( parent.isValid() ) // Points to an actual parent object.
    {
        // Check that the row/col is valid on the parent.
        QObject* p = getItem(parent);
        if ( parent.column() != 1 || !isValidForParentIndex(p, parent.row()) ) return QModelIndex();

        // The row/col combination does actually refer to a child slot on the parent.
        // Get this child.
        QObject* c = p->children().at(rowToChildIndex(p, row));

        int propertyCount = c->metaObject()->propertyCount();
        int childCount = c->children().count();
        if ( row >= propertyCount + childCount ) return QModelIndex();

        // Return an index with the internal pointer pointing to this object,
        // and the row/col set accordingly.
        return createIndex(row, column, c);
    }
    else    // Points to invisible root.
    {
        int propertyCount = m_pRootItem->metaObject()->propertyCount();
        int childCount = m_pRootItem->children().count();
        if ( row >= propertyCount + childCount ) return QModelIndex();

        return createIndex(row, column, m_pRootItem);
    }
}

QModelIndex ModelTreeA::parent(const QModelIndex &child) const
{
    QObject* obj = getItem(child);

    // If the object or its parent is the invisible root, return an invalid index.
    if ( obj == m_pRootItem || obj->parent() == m_pRootItem ) return QModelIndex();

    // Get the index of the object within its QObject parent.
    QObject* parent = obj->parent();
    Q_ASSERT(parent);
    int indexInParent = parent->children().indexOf(obj);

    // Add on propertyCount().
    indexInParent += parent->metaObject()->propertyCount();

    // Return an index.
    return createIndex(indexInParent, 1, parent);
}

Qt::ItemFlags ModelTreeA::flags(const QModelIndex &index) const
{
    return index.isValid() ? QAbstractItemModel::flags(index) : Qt::NoItemFlags;
}

int ModelTreeA::rowCount(const QObject *obj)
{
    return obj->metaObject()->propertyCount() + obj->children().count();
}

int ModelTreeA::rowCount(const QModelIndex &parent) const
{
    return rowCount(getItem(parent));
}

int ModelTreeA::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ModelTreeA::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole ) return QVariant();

    if ( orientation == Qt::Horizontal )
    {
        switch (section)
        {
        case 0:
            return QVariant(QString("Key"));

        case 1:
            return QVariant(QString("Value"));

        default:
            return QVariant();
        }
    }

    return QVariant(section);
}

QVariant ModelTreeA::data(const QModelIndex &index, int role) const
{
    if ( role != Qt::DisplayRole ) return QVariant();

    QObject* obj = getItem(index);

    if ( index.column() < 0 || index.column() > 1 || index.row() < 0 ) return QVariant();

    if ( index.column() == 0 )
    {
        if ( index.row() < obj->metaObject()->propertyCount() )
        {
            return QVariant(propertyName(obj, index.row()));
        }
        else
        {
            int childIndex = rowToChildIndex(obj, index.row());
            if ( childIndex > obj->children().count() ) return QVariant();
            return QVariant(QString("child%0").arg(childIndex));
        }
    }
    else
    {
        if ( index.row() < obj->metaObject()->propertyCount() )
        {
            return propertyValue(obj, index.row());
        }
        else
        {
            int childIndex = rowToChildIndex(obj, index.row());
            QList<QObject*> children = obj->children();
            if ( childIndex > children.count() ) return QVariant();
            return QVariant(children.at(childIndex));
        }
    }
}

QString ModelTreeA::propertyName(const QObject *obj, int propertyIndex)
{
    return QString(obj->metaObject()->property(propertyIndex).name());
}

QVariant ModelTreeA::propertyValue(const QObject *obj, int propertyIndex)
{
    return obj->metaObject()->property(propertyIndex).read(obj);
}
