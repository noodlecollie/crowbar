#include "databasenode.h"

MODEL_BEGIN_NAMESPACE

DatabaseNode::DatabaseNode(DatabaseNode *parent)
{
    m_pParent = parent;
}

DatabaseNode::DatabaseNode(const QVariant &key, const QVariant &value, DatabaseNode *parent)
{
    m_pParent = parent;
    m_vKey = key;
    m_vValue = value;
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

bool DatabaseNode::insertChildren(int position, int count)
{
    // Don't insert if the index is invalid.
    if ( position < 0 || position > m_Children.count() ) return false;

    m_vValue.clear();

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

QVariant DatabaseNode::data(int column) const
{
    if ( column < 0 || column > 1 ) return QVariant();

    return column == 0 ? key() : value();
}

bool DatabaseNode::setData(int column, const QVariant &data)
{
    if ( column < 0 || column > 1 ) return false;

    if ( column == 0 ) setKey(data);
    else setValue(data);
    return true;
}

QVariant DatabaseNode::key() const
{
    return m_vKey;
}

void DatabaseNode::setKey(const QVariant &key)
{
    m_vKey = key;
}

QVariant DatabaseNode::value() const
{
    return isLeaf() ? m_vValue : QVariant();
}

void DatabaseNode::setValue(const QVariant &value)
{
    if ( isLeaf() ) m_vValue = value;
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
