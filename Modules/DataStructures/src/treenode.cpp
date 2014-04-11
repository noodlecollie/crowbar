#include "treenode.h"

DATASTRUCTURES_BEGIN_NAMESPACE

TreeNode::TreeNode(ITreeNode* parent) :
    m_Children(), m_pParent(parent)
{
}

TreeNode::~TreeNode()
{
    // Delete each of our children.
    // If we're a leaf this won't do anything anyway.
    pruneSubtree();
}

void TreeNode::addChild(ITreeNode *node)
{
    // We'd like to know if NULL was passed, because it shouldn't be.
    Q_ASSERT( node );
    if ( !node ) return;
    
    // Check the child doesn't already exist!
    if ( dynamic_cast<TreeNode*>(node) == this || m_Children.contains(node) ) return;
    
    // Add the child to the list.
    m_Children.append(node);
    
    // Set the parent of the child to this node.
    node->setParent(this);
}

ITreeNode* TreeNode::addChild()
{
    // Create a new child and add it to the list.
    TreeNode* node = new TreeNode();
    addChild(node);
    
    return node;
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

void TreeNode::removeChild(ITreeNode *node)
{
    if ( !node ) return;
    
    for ( int i = 0; i < m_Children.size(); i++ )
    {
        if ( m_Children.at(i) == node )
        {
            m_Children.removeAt(i);
            return;
        }
    }
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
    
    const ITreeNode* const me = dynamic_cast<const ITreeNode* const>(this);
    
    // If the node has a parent:
    while ( node->parent() )
    {
        // Check whether the parent is this node.
        if ( dynamic_cast<const ITreeNode* const>(node->parent()) == me ) return true;
        
        // Set the new node to the parent and loop.
        node = node->parent();
    }
    
    // The node has no parent, so if it is not equal to us then we are not an ancestor.
    if ( dynamic_cast<const ITreeNode* const>(node) == me ) return true;
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

void TreeNode::pruneSubtree()
{
    // If we're a leaf, return.
    if ( isLeaf() ) return;
    
    // For each child (from the end of the list so we don't keep shifting items):
    while( !m_Children.empty() )
    {
        // Call pruneSubtree() on each child.
        ITreeNode* node = m_Children.takeLast();
        node->pruneSubtree();
        
        // The child has now deleted all of its own children, so we can delete it.
        delete node;
    }
}

QList<ITreeNode*> TreeNode::detachNode()
{
    if ( !isRoot() )
    {
        // Remove this node from its parent.
        m_pParent->removeChild(this);
        m_pParent = NULL;
    }
    
    QList<ITreeNode*> list;
    
    while( !m_Children.empty() )
    {
        // Remove the child from this node.
        ITreeNode* node = m_Children.takeLast();
        Q_ASSERT( node );
        node->setParent(NULL);
        list.append(node);
    }
    
    return list;
}

DATASTRUCTURES_END_NAMESPACE
