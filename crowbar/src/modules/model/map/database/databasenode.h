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
    ~DatabaseNode();

    // Child management
    DatabaseNode* childAt(int index) const;
    void clearChildren();
    int childCount() const;
    bool insertChildren(int position, int count, bool* valueNullified = NULL);
    bool removeChildren(int position, int count);
    inline bool isLeaf() const { return m_Children.count() < 1; }
    int indexInParent() const;

    // Parent
    DatabaseNode* parentNode() const;

    // Data
    QVariant key(int role) const;
    bool setKey(const QVariant &key, int role);
    QVariant value(int role) const;
    bool setValue(const QVariant &value, int role);
    QVariant data(int column, int role) const;
    bool setData(int column, const QVariant &data, int role);

private:
    QList<DatabaseNode*> m_Children;
    QVariant m_vKey;
    QVariant m_vValue;
    DatabaseNode* m_pParent;
};

MODEL_END_NAMESPACE

#endif // DATABASENODE_H
