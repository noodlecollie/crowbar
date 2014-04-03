/** \file treenode.h
 * \brief Defines a base tree node class.
 */

#ifndef TREENODE_H
#define TREENODE_H

#include "octree_global.h"
#include "itreenode.h"
#include <QList>

OCTREE_BEGIN_NAMESPACE

/**
 * @brief Defines a tree node that implements ITreeNode.
 * 
 * TreeNode is the base class for more specialised subclasses. It provides basic functionality for
 * recursion to act upon, including storing/removing child nodes.
 */
class OCTREESHARED_EXPORT TreeNode : public virtual ITreeNode
{
public:
    /**
     * @brief Constructor.
     * @param parent Parent node. Note that this does not explicitly add this node as a child inside the parent node.
     */
    explicit TreeNode(ITreeNode* parent = NULL);

    /**
     * @brief Destructor. This node's children are recursively deleted.
     */
    virtual ~TreeNode();

    // ITreeNode
    /**
     * @brief Adds a child node to this node. The node takes ownership of the child.
     * @note The child should be a TreeNode or better, otherwise the add will fail.
     * @param node Node to add.
     */
    virtual void addChild(ITreeNode *node);

    /**
     * @brief Removes the child node at the given index from this node. The caller takes ownership of the child.
     * @param index Index of child to remove.
     * @return Child removed, or NULL if the index was invalid.
     */
    virtual ITreeNode* removeChild(int index);

    /**
     * @brief Removes the given child node if it exists.
     * @param node Node to remove.
     */
    virtual void removeChild(ITreeNode* node);

    /**
     * @brief Recursively deletes all of this node's children, leaving this node empty.
     */
    virtual void pruneSubtree();

    /**
     * @brief Returns the child at a given index. This node retains ownership of the child.
     * @param index Index of the child.
     * @return Child at this index, or NULL if the index was invalid.
     */
    virtual ITreeNode* childAt(int index) const;

    /**
     * @brief Returns whether the given node is recorded as a child of this node.
     * @param node Node to search for.
     * @return True if the given node is recorded as a child; false otherwise, or if the node provided is NULL.
     */
    virtual bool containsChild(ITreeNode *node) const;

    /**
     * @brief Returns whether this node is an ancestor of the given node.
     * @note This means that this node is present further up the path from the root node to the given node.
     * @param node Node to check ancestry of.
     * @return True if this node is an ancestor of the given node, false otherwise or if the given node is NULL.
     */
    virtual bool isAncestor(const ITreeNode *node) const;

    /**
     * @brief Returns whether this node is a successor of the given node.
     * @note This means that the given node is present further up the path from the root node to this node.
     * @param node Node to check successors of.
     * @return True if this node is a successor of the given node, false otherwise or if the given node is NULL.
     */
    virtual bool isSuccessor(const ITreeNode *node) const;

    /**
     * @brief Returns the number of direct children this node has
     * @return Number of children recorded in this node.
     */
    virtual int childCount() const;

    /**
     * @brief Returns whether this node is a leaf (ie. it has no children).
     * @return True if the node is a leaf, false otherwise.
     */
    virtual bool isLeaf() const;

    /**
     * @brief Returns a pointer to the parent of this node.
     * @return Pointer to parent, or NULL if no parent is recorded.
     */
    virtual ITreeNode* parent() const;

    /**
     * @brief Sets the parent of this node.
     * @param parent Parent node to set.
     */
    virtual void setParent(ITreeNode *parent);

    /**
     * @brief Returns whether this node is the root (ie. it has no parent).
     * @return True if the node is the root of the tree, false otherwise.
     */
    virtual bool isRoot() const;
    
    /**
     * @brief Removes the node from the tree structure, leaving the remainder disjoint.
     * 
     * The node will be removed as a child from its parent, and will leave all its children as roots of
     * their respective subtrees.
     */
    virtual QList<ITreeNode*> detachNode();

private:
    QList<ITreeNode*>   m_Children;  /**< Array of child nodes. */
    ITreeNode*          m_pParent;   /**< Parent of this node. */
};

OCTREE_END_NAMESPACE

#endif // TREENODE_H
