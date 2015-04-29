#ifndef DATABASENODE_H
#define DATABASENODE_H

#include "model_global.h"
#include <QList>
#include <QVariant>

MODEL_BEGIN_NAMESPACE

// Number of columns = 2: one for key, one for value.
// If not a leaf, value should not be used.

class MODELSHARED_EXPORT DatabaseNode
{
    friend class DatabaseNode;
public:
    DatabaseNode(DatabaseNode* parent = 0);
    DatabaseNode(const QVariant &key, const QVariant &value, DatabaseNode* parent = 0);
    ~DatabaseNode();

    // Child management
    DatabaseNode* childAt(int index) const;
    void clearChildren();
    int childCount() const;
    bool insertChildren(int position, int count);
    bool removeChildren(int position, int count);
    inline bool isLeaf() const { return m_Children.count() < 1; }
    int indexInParent() const;

    // Parent
    DatabaseNode* parentNode() const;

    // Data
    QVariant key() const;
    void setKey(const QVariant &key);
    QVariant value() const;
    void setValue(const QVariant &value);
    QVariant data(int column) const;
    bool setData(int column, const QVariant &data);

private:
    QList<DatabaseNode*> m_Children;
    QVariant m_vKey;
    QVariant m_vValue;
    DatabaseNode* m_pParent;
};

MODEL_END_NAMESPACE

#endif // DATABASENODE_H
