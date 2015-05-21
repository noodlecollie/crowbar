#include "modeltreea.h"
#include <QMetaProperty>
#include <QtDebug>

#define MAX_COLUMN 2
#define COLUMN_PROPERTY_VALUE 2

ModelTreeA::ModelTreeA(QObject *parent) : QAbstractItemModel(parent)
{
    m_pRootItem = NULL;
    m_bOwnsRoot = false;
    m_bDesignablePropertiesOnly = false;
}

ModelTreeA::~ModelTreeA()
{
    if ( m_bOwnsRoot ) delete m_pRootItem;
}

void ModelTreeA::setRoot(QObject *obj)
{
    if ( !obj ) return;
    if ( m_bOwnsRoot ) delete m_pRootItem;
    m_pRootItem = obj;
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
    if ( row < 0 || column < 0 || column > MAX_COLUMN ) return QModelIndex();
    
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
    return MAX_COLUMN + 1;
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
    // Validate.
    if ( !index.isValid() || index.row() < 0 || index.column() < 0 || index.column() > MAX_COLUMN ) return QAbstractItemModel::flags(index);

    // Get the object that owns the index.
    QObject* obj = ownerObject(index);
    if ( !obj ) return QAbstractItemModel::flags(index);

    // If the row refers to a child, just return the default (for normal text).
    if ( index.row() >= obj->metaObject()->propertyCount() ) return QAbstractItemModel::flags(index);

    // If we're not limited to designable properties, return editable if the column is correct.
    if ( !m_bDesignablePropertiesOnly )
        return (index.column() == COLUMN_PROPERTY_VALUE)
            ? (Qt::ItemIsEditable | QAbstractItemModel::flags(index))
            : QAbstractItemModel::flags(index);
    
    // If the property is designable, return editable if the column is correct.
    if ( obj->metaObject()->property(index.row()).isDesignable() )
    {
        return (index.column() == COLUMN_PROPERTY_VALUE)
            ? (Qt::ItemIsEditable | QAbstractItemModel::flags(index))
            : QAbstractItemModel::flags(index);
    }
    
    // Otherwise return no flags (greyed out).
    else
    {
        return Qt::NoItemFlags;
    }
}

QVariant ModelTreeA::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || section < 0 || section > MAX_COLUMN || role != Qt::DisplayRole) return QVariant();

    switch (section)
    {
    case 0:
        return QVariant(QString("Property"));

    case 1:
        return QVariant(QString("Type"));

    case 2:
        return QVariant(QString("Value"));

    default:
        Q_ASSERT(false);
        return QVariant();
    }
}

QVariant ModelTreeA::data(const QModelIndex &index, int role) const
{
    // Validate.
    if ( !index.isValid() || index.row() < 0 || index.column() < 0 || index.column() > MAX_COLUMN ) return QVariant();
    if ( role != Qt::DisplayRole && role != Qt::EditRole ) return QVariant();
    
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
    switch (index.column())
    {
    case 0: // Name of property.
        return QVariant(QString(obj->metaObject()->property(index.row()).name()));

    case 1: // Type of property.
        return QVariant(QString(obj->metaObject()->property(index.row()).typeName()));

    case 2: // Value of property.
        return obj->metaObject()->property(index.row()).read(obj);

    default:
        Q_ASSERT(false);
        return QVariant();
    }
}

bool ModelTreeA::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // Validate.
    if ( !index.isValid() || role != Qt::EditRole || index.row() < 0 || index.column() < 0 || index.column() > MAX_COLUMN ) return false;

    // Get the object that owns the index.
    QObject* obj = ownerObject(index);
    if ( !obj ) return false;

    // If the row refers to a child, we can't edit this right now.
    if ( isChildRow(obj, index.row()) ) return false;

    // Make sure the column is valid.
    if ( index.column() != COLUMN_PROPERTY_VALUE ) return false;

    // Set the value.
    bool result = obj->metaObject()->property(index.row()).write(obj, value);
    if ( result )
    {
        emit dataChanged(index, index, QVector<int>(1, role));
    }
    return result;
}

bool ModelTreeA::ownsRootItem() const
{
    return m_bOwnsRoot;
}

void ModelTreeA::setOwnsRootItem(bool owns)
{
    if ( owns == m_bOwnsRoot ) return;
    
    m_bOwnsRoot = owns;
}

bool ModelTreeA::designablePropertiesOnly() const
{
    return m_bDesignablePropertiesOnly;
}

void ModelTreeA::setDesignablePropertiesOnly(bool enabled)
{
    if ( m_bDesignablePropertiesOnly == enabled ) return;
    
    m_bDesignablePropertiesOnly = enabled;
}
