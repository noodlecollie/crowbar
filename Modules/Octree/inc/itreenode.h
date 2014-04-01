#ifndef ITREENODE_H
#define ITREENODE_H

#include "octree_global.h"

OCTREE_BEGIN_NAMESPACE

/**
 * @brief Interface for a recursive tree node.
 */
class OCTREESHARED_EXPORT ITreeNode
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~ITreeNode() {}
    
    /**
     * @brief Adds a child node to this node.
     * @param node Node to add.
     */
    virtual void addChild(ITreeNode* node) = 0;
    
    /**
     * @brief Removes the child node at the given index from this node.
     * @param index Index of child to remove.
     * @return Child removed, or NULL if the index was invalid.
     */
    virtual ITreeNode* removeChild(int index) = 0;
    
    /**
     * @brief Returns the child at a given index.
     * @param index Index of the child.
     * @return Child at this index, or NULL if the index was invalid.
     */
    virtual ITreeNode* childAt(int index) const = 0;
    
    /**
     * @brief Returns whether the given node is recorded as a child of this node.
     * @param node Node to search for.
     * @return True if the given node is recorded as a child; false otherwise, or if the node provided is NULL.
     */
    virtual bool containsChild(ITreeNode* node) const = 0;
    
    /**
     * @brief Returns whether this node is an ancestor of the given node.
     * @note This means that this node is present further up the path from the root node to the given node.
     * @param node Node to check ancestry of.
     * @return True if this node is an ancestor of the given node, false otherwise or if the given node is NULL.
     */
    virtual bool isAncestor(const ITreeNode* node) const = 0;
    
    /**
     * @brief Returns whether this node is a successor of the given node.
     * @note This means that the given node is present further up the path from the root node to this node.
     * @param node Node to check successors of.
     * @return True if this node is a successor of the given node, false otherwise or if the given node is NULL.
     */
    virtual bool isSuccessor(const ITreeNode *node) const = 0;
    
    /**
     * @brief Returns the number of direct children this node has
     * @return Number of children recorded in this node.
     */
    virtual int childCount() const = 0;
    
    /**
     * @brief Returns whether this node is a leaf (ie. it has no children).
     * @return True if the node is a leaf, false otherwise.
     */
    virtual bool isLeaf() const  = 0;
    
    /**
     * @brief Returns a pointer to the parent of this node.
     * @return Pointer to parent, or NULL if no parent is recorded.
     */
    virtual ITreeNode* parent() const = 0;
    
    /**
     * @brief Sets the parent of this node.
     * @param parent Parent node to set.
     */
    virtual void setParent(ITreeNode* parent) = 0;
    
    /**
     * @brief Returns whether this node is the root (ie. it has no parent).
     * @return True if the node is the root of the tree, false otherwise.
     */
    virtual bool isRoot() const = 0;
};

OCTREE_END_NAMESPACE

#endif // ITREENODE_H
