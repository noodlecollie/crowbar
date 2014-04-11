#ifndef KEYVALUESNODE_H
#define KEYVALUESNODE_H

#include "datastructures_global.h"
#include "treenode.h"

#include <QHash>
#include <QString>
#include <QVariant>
#include <QObject>
#include <QVector2D>
#include <QVector3D>

// For convenience - we -shouldn't- need dynamic_cast if we're only able to add/remove children as
// KeyValuesNode and not the more abstract ITreeNode, but I'm defining this here in case it'd
// be useful to turn dynamic_casts on or off.
#define KVN_USE_DYN_CAST
#ifdef KVN_USE_DYN_CAST
// Use dynamic_cast to check the type of the child we're returning at runtime.
#define KVN_CAST dynamic_cast<KeyValuesNode*>
#else
// Just use an old C-style cast.
#define KVN_CAST (KeyValuesNode*)
#endif

DATASTRUCTURES_BEGIN_NAMESPACE

class DATASTRUCTURESSHARED_EXPORT KeyValuesNode : public QObject, public TreeNode
{
    Q_OBJECT
public:
    KeyValuesNode(KeyValuesNode* parent = NULL);
    virtual ~KeyValuesNode();

    QString key() const;
    void setKey(const QString &key);
    void setKey(const char* str);
    
    QVariant value() const;
    const QVariant& constValue() const;
    void setValue(const QVariant &value);
    void setValue(const QString &value);
    void setValue(float value);
    void setValue(bool value);
    void setValue(int value);
    void setValue(const QVector3D &value);
    void setValue(const QVector2D &value);
    bool hasValue() const;
    
    // Convenience overrides.
    virtual void addChild(KeyValuesNode* node)         { TreeNode::addChild(node); }
    virtual KeyValuesNode* removeChild(int index)      { return KVN_CAST(TreeNode::removeChild(index)); }
    virtual KeyValuesNode* childAt(int index) const    { return KVN_CAST(TreeNode::childAt(index)); }
    virtual KeyValuesNode* parent() const              { return KVN_CAST(TreeNode::parent()); }
    virtual void setParent(KeyValuesNode* parent)      { TreeNode::setParent(parent); }
    virtual KeyValuesNode* addChild()
    {
        KeyValuesNode* node = new KeyValuesNode();
        addChild(node);
        return node;
    }

private:
    QString     m_szKey;
    QVariant    m_Value;
};

DATASTRUCTURES_END_NAMESPACE

// Undef the macro:
#undef KVN_CAST

#endif // KEYVALUESNODE_H
