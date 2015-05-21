#include "modeltreea.h"
#include <QMetaProperty>
#include <QtDebug>

ModelTreeA::ModelTreeA(QObject *parent) : QAbstractItemModel(parent)
{
    m_pRootItem = new QObject(this);
}

ModelTreeA::~ModelTreeA()
{
    // Everything will be deleted when the root item is deleted.
}

void ModelTreeA::setRoot(QObject *obj)
{
    if ( !obj ) return;
    delete m_pRootItem;
    m_pRootItem = obj;
    m_pRootItem->setParent(this);
}

QObject* ModelTreeA::childAt(const QModelIndex &index) const
{
    if ( !index.isValid() ) return m_pRootItem;

    // Children only reside under column 0.
    if ( index.column() != 0 ) return NULL;
    
    // Get the object that owns the row pointed to by the index.
    QObject* obj = ownerObject(index);
    
    // If there is no owner, return NULL.
    if ( !obj ) return NULL;
    
    // Convert the row to a child index on the owner object.
    int i = childIndex(obj, index.row());
    
    // If the index is outside the range of children on the owner, return NULL.
    if ( i < 0 || i >= obj->children().count() ) return NULL;
    
    // Otherwise, return the child at this index.
    return obj->children().at(i);
}

// Given a row, column and parent, return an index with a correctly set internal pointer.
QModelIndex ModelTreeA::index(int row, int column, const QModelIndex &parent) const
{
    // If the row or column is invalid, return an invalid index.
    if ( row < 0 || column < 0 || column > 1 ) return QModelIndex();
    
    // We want to get the child QObject that is held under the parent index.
    QObject* child = childAt(parent);
    if ( !child ) return QModelIndex();
    
    // Now validate the row and column.
    // Not sure this is strictly necessary but let's be safe.
    if ( row > totalRowCount(child) ) return QModelIndex();
    
    // If the row refers to a child, ensure the column is 0.
    if ( isChildRow(child, row) )
    {
        return column == 0 ? createIndex(row, column, child) : QModelIndex();
    }
    
    // If not, we must refer to a property and validation is already done.
    return createIndex(row, column, child);
}

QModelIndex ModelTreeA::parent(const QModelIndex &child) const
{
    // Invalid indices have no parent.
    if ( !child.isValid() ) return QModelIndex();
    
    // Get the QObject that owns the index.
    QObject* obj = ownerObject(child);
    
    // If there is no object, or it's the root, return an invalid index.
    if ( !obj || obj == m_pRootItem ) return QModelIndex();
    
    // Get the parent of this object.
    QObject* parent = obj->parent();
    
    // If it doesn't exist, return an invalid index.
    if ( !parent ) return QModelIndex();
    
    // Get the index of the child object within the parent.
    int chIndex = parent->children().indexOf(obj);
    Q_ASSERT(chIndex >= 0);
    
    // Create an index. The column is set to 0.
    return createIndex(chIndex + parent->metaObject()->propertyCount(), 0, parent);
}

int ModelTreeA::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

int ModelTreeA::rowCount(const QModelIndex &parent) const
{
    // Get the object under this index.
    QObject* obj = childAt(parent);
    
    // If there is no child object, return 0 rows.
    if ( !obj ) return 0;
    
    // Return the total number of rows on this object.
    return totalRowCount(obj);
}

Qt::ItemFlags ModelTreeA::flags(const QModelIndex &index) const
{
    // Right now we don't allow any editing whatsoever.
    Q_UNUSED(index);
    return Qt::NoItemFlags;
}

QVariant ModelTreeA::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || section < 0 || section > 1 || role != Qt::DisplayRole) return QVariant();
    return section == 0 ? QVariant(QString("Key")) : QVariant(QString("Value"));
}

QVariant ModelTreeA::data(const QModelIndex &index, int role) const
{
    // Validate.
    if ( !index.isValid() || role != Qt::DisplayRole || index.row() < 0 || index.column() < 0 || index.column() > 1 ) return QVariant();
    
    // Get the object that owns the index.
    QObject* obj = ownerObject(index);
    if ( !obj ) return QVariant();
    
    // Now validate the row and column.
    // Not sure this is strictly necessary but let's be safe.
    if ( index.row() > totalRowCount(obj) ) return QVariant();
    
    // If the row refers to a child, ensure the column is 0.
    if ( isChildRow(obj, index.row()) )
    {
        return index.column() == 0 ? QVariant(QString("Child %0").arg(childIndex(obj, index.row()))) : QVariant();
    }
    
    // If not, we must refer to a property.
    if ( index.column() == 0 )
    {
        // Return the name.
        return QVariant(QString(obj->metaObject()->property(index.row()).name()));
    }
    else
    {
        // Return the value.
        return obj->metaObject()->property(index.row()).read(obj);
    }

    return role == Qt::DisplayRole ? QVariant(QString("Display")) : QVariant();
}
