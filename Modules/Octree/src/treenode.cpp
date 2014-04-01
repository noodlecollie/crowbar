#include "treenode.h"

OCTREE_BEGIN_NAMESPACE

TreeNode::TreeNode(ITreeNode* parent) :
    m_Children(), m_pParent(parent)
{
}

TreeNode::~TreeNode()
{
}

void TreeNode::addChild(ITreeNode *node)
{
    // Add the child to the list.
    m_Children.append(node);
    
    // Set the parent of the child to this node.
    node->setParent(this);
}

ITreeNode* TreeNode::removeChild(int index)
{
    // Validate the index.
    Q_ASSERT( index >= 0 && index < m_Children.size() );
    if ( index < 0 || index >= m_Children.size() ) return NULL;
    
    // Get the child.
    ITreeNode* node = m_Children.takeAt(index);
    
    // Set its parent to NULL.
    node->setParent(NULL);
    
    // Return the child.
    return node;
}

ITreeNode* TreeNode::childAt(int index) const
{
    // Validate the index.
    Q_ASSERT( index >= 0 && index < m_Children.size() );
    if ( index < 0 || index >= m_Children.size() ) return NULL;
    
    // Return the child at this index.
    return m_Children.at(index);
}

bool TreeNode::containsChild(ITreeNode *node) const
{
    // If the node is NULL, return false.
    if ( !node ) return false;
    
    // Find the node in the list.
    return m_Children.contains(node);
}

bool TreeNode::isAncestor(const ITreeNode *node) const
{
    if ( !node ) return false;
    
    // The quickest way to do this is to run up the tree from the
    // target node to the root, as the branching factor in this
    // direction is 1.
    
    // If the node has a parent:
    while ( node->parent() )
    {
        // Check whether the parent is this node.
        if ( node->parent() == this ) return true;
        
        // Set the new node to the parent and loop.
        node = node->parent();
    }
    
    // The node has no parent, so if it is not equal to us then we are not an ancestor.
    if ( node == this ) return true;
    return false;
}

bool TreeNode::isSuccessor(const ITreeNode *node) const
{
    if ( !node ) return false;
    
    // The quickest way to do this is going to be to call isAncestor()
    // on the other node since A <ancestor of> B <=> B <successor of> A.
    return node->isAncestor(this);
}

int TreeNode::childCount() const
{
    return m_Children.size();
}

bool TreeNode::isLeaf() const
{
    // We are a leaf if we have no children.
    return childCount() < 1;
}

ITreeNode* TreeNode::parent() const
{
    return m_pParent;
}

void TreeNode::setParent(ITreeNode *parent)
{
    m_pParent = parent;
}

bool TreeNode::isRoot() const
{
    // Note that this only comes from this node's point of view:
    // some other node could have added this one without its knowledge!
    return parent() == NULL;
}

void TreeNode::testFunc() const
{
    
}

OCTREE_END_NAMESPACE
