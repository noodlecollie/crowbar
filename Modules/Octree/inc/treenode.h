#ifndef TREENODE_H
#define TREENODE_H

#include "octree_global.h"
#include "itreenode.h"
#include <QList>

OCTREE_BEGIN_NAMESPACE

class OCTREESHARED_EXPORT TreeNode : public ITreeNode
{
public:
    explicit TreeNode(ITreeNode* parent = NULL);
    virtual ~TreeNode();

    // ITreeNode
    virtual void addChild(ITreeNode *node);
    virtual ITreeNode* removeChild(int index);
    virtual ITreeNode* childAt(int index) const;
    virtual bool containsChild(ITreeNode *node) const;
    virtual bool isAncestor(const ITreeNode *node) const;
    virtual bool isSuccessor(const ITreeNode *node) const;
    virtual int childCount() const;
    virtual bool isLeaf() const;
    virtual ITreeNode* parent() const;
    virtual void setParent(ITreeNode *parent);
    virtual bool isRoot() const;

    void testFunc() const;

private:
    QList<ITreeNode*>  m_Children;
    ITreeNode*         m_pParent;
};

OCTREE_END_NAMESPACE

#endif // TREENODE_H
