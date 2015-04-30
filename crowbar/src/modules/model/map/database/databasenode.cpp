#include "databasenode.h"

MODEL_BEGIN_NAMESPACE

DatabaseNode::DatabaseNode(DatabaseNode *parent)
{
    m_pParent = parent;
}

DatabaseNode::~DatabaseNode()
{
    clearChildren();
}

DatabaseNode* DatabaseNode::parentNode() const
{
    return m_pParent;
}

int DatabaseNode::childCount() const
{
    return m_Children.count();
}

DatabaseNode* DatabaseNode::childAt(int index) const
{
    return m_Children.at(index);
}

bool DatabaseNode::insertChildren(int position, int count, bool *valueNullified)
{
    // Don't insert if the index is invalid.
    if ( position < 0 || position > m_Children.count() ) return false;

    // If we're becoming a non-leaf, make our value invalid and notify the caller.
    if ( isLeaf() && count > 0 )
    {
        m_vValue.clear();
        if ( valueNullified ) *valueNullified = true;
    }
    else if ( valueNullified ) *valueNullified = false;

    for (int row = 0; row < count; ++row)
    {
        DatabaseNode* node = new DatabaseNode(this);
        m_Children.insert(position, node);
    }

    return true;
}

bool DatabaseNode::removeChildren(int position, int count)
{
    // Don't remove if the index is invalid.
    if (position < 0 || position + count > m_Children.count()) return false;

    for (int row = 0; row < count; ++row) delete m_Children.takeAt(position);

    return true;
}

QVariant DatabaseNode::data(int column, int role) const
{
    if ( column < 0 || column > 1 ) return QVariant();

    return column == 0 ? key(role) : value(role);
}

bool DatabaseNode::setData(int column, const QVariant &data, int role)
{
    if ( column < 0 || column > 1 ) return false;

    if ( column == 0 ) return setKey(data, role);
    else return setValue(data, role);
}

QVariant DatabaseNode::key(int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        return m_vKey;

    default:
        return QVariant();
    }
}

bool DatabaseNode::setKey(const QVariant &key, int role)
{
    switch (role)
    {
    case Qt::EditRole:
        m_vKey = key;
        return true;

    default:
        return false;
    }
}

QVariant DatabaseNode::value(int role) const
{
    switch ( role )
    {
    case Qt::DisplayRole:
        return isLeaf() ? m_vValue : QVariant();

    default:
        return QVariant();
    }
}

bool DatabaseNode::setValue(const QVariant &value, int role)
{
    switch (role)
    {
    case Qt::EditRole:
        if ( isLeaf() )
        {
            m_vValue = value;
            return true;
        }

        return false;

    default:
        return false;
    }
}

int DatabaseNode::indexInParent() const
{
    return m_pParent ? m_pParent->m_Children.indexOf(const_cast<DatabaseNode*>(this)) : -1;
}

void DatabaseNode::clearChildren()
{
    qDeleteAll(m_Children);
}

MODEL_END_NAMESPACE
