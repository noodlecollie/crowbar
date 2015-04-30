#include "database.h"
#include "databasenode.h"

MODEL_BEGIN_NAMESPACE

Database::Database(QObject *parent) : QAbstractItemModel(parent)
{
    m_pRootItem = new DatabaseNode();
    m_vHeaderKey.setValue(QString("Key"));
    m_vHeaderValue.setValue(QString("Value"));
}

Database::~Database()
{
    delete m_pRootItem;
}

QVariant Database::data(const QModelIndex &index, int role) const
{
    // If the index is invalid, don't get anything.
    if ( !index.isValid() ) return QVariant();

    // Get the node.
    DatabaseNode* node = getNode(index);

    // Return its data.
    Q_ASSERT(index.column() == 0 || index.column() == 1);
    return node->data(index.column(), role);
}

QVariant Database::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole || section < 0 || section > 1 ) return QVariant();

    return section == 0 ? QVariant(QString("Key")) : QVariant(QString("Value"));
}

int Database::rowCount(const QModelIndex &parent) const
{
    DatabaseNode* node = getNode(parent);
    return node->childCount();
}

int Database::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QModelIndex Database::index(int row, int column, const QModelIndex &parent) const
{
    // This function generates a model index, which is basically just a compact representation
    // of the data passed in. We use the pointer within the parent, along with the row and
    // column values, to create the index. The index stores the row, column and an internal
    // pointer to the actual DatabaseNode.

    // In a nutshell, all this function does is translates the row and column numbers
    // from the parent node to the corresponding child node.

    // First of all, we only want to return an index if the column is 0 or 1.
    if ( column < 0 || column > 1 ) return QModelIndex();

    // Get the node corresponding to the parent.
    DatabaseNode* parentNode = getNode(parent);

    // Get the actual node.
    DatabaseNode* node = (row >= 0 && row < parentNode->childCount()) ? parentNode->childAt(row) : NULL;

    return node ? createIndex(row, column, node) : QModelIndex();
}

QModelIndex Database::parent(const QModelIndex &child) const
{
    if ( !child.isValid() ) return QModelIndex();

    DatabaseNode* childNode = getNode(child);
    DatabaseNode* parentNode = childNode->parentNode();
    if ( !parentNode || parentNode == m_pRootItem ) return QModelIndex();
    return createIndex(parentNode->indexInParent(), 0, parentNode);
}

bool Database::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // If the index is invalid, don't set anything.
    if ( !index.isValid() ) return false;

    // Get the node pointed to by the index.
    DatabaseNode* node = getNode(index);

    // Set the value on this node in the corresponding column.
    // We don't need to worry about the row, since the pointer
    // in the index corresponds to the item at that row.
    Q_ASSERT(index.column() == 0 || index.column() == 1);   // Let us know if the column is wrong.
    bool result = node->setData(index.column(), value, role);
    if ( result )
    {
        emit dataChanged(index, index, QVector<int>(1, role));
    }

    return result;
}

bool Database::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    // TODO: Incomplete.
    return false;
}

bool Database::insertColumns(int column, int count, const QModelIndex &parent)
{
    // Columns are immutable.
    Q_UNUSED(column);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    return false;
}

bool Database::removeColumns(int column, int count, const QModelIndex &parent)
{
    // Columns are immutable.
    Q_UNUSED(column);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    return false;
}

bool Database::insertRows(int row, int count, const QModelIndex &parent)
{
    DatabaseNode* parentNode = getNode(parent);
    bool success = false;

    beginInsertRows(parent, row, row + count - 1);
    bool valueNullified = false;
    success = parentNode->insertChildren(row, count, &valueNullified);
    endInsertRows();

    if ( valueNullified ) emit dataChanged(parent, parent);

    return success;
}

bool Database::removeRows(int row, int count, const QModelIndex &parent)
{
    DatabaseNode* parentNode = getNode(parent);
    bool success = false;

    beginRemoveRows(parent, row, row + count - 1);
    success = parentNode->removeChildren(row, count);
    endRemoveRows();

    return success;
}

DatabaseNode* Database::getNode(const QModelIndex &index) const
{
    if ( index.isValid() )
    {
        DatabaseNode *node = static_cast<DatabaseNode*>(index.internalPointer());
        if ( node ) return node;
    }

    return m_pRootItem;
}

Qt::ItemFlags Database::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;

    DatabaseNode* node = getNode(index);
    if ( !node->isLeaf() && index.column() != 0 ) return QAbstractItemModel::flags(index);

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

MODEL_END_NAMESPACE
