/** \file worldculltreenode.h
 * \brief Defines the WorldCullTreeNode class.
 */

#ifndef WORLDCULLTREENODE_H
#define WORLDCULLTREENODE_H

#include "datastructures_global.h"
#include "treenode.h"
#include <QHash>
#include <Qt3D/QBox3D>

DATASTRUCTURES_BEGIN_NAMESPACE

#define MIN_DIMENSION   1024.0f
#define MIN_OBJECTS     2

/* The saga of this class: http://stackoverflow.com/questions/22783851/c-calling-derived-specialised-virtual-functions-within-a-base-template-class/
 * For reference if I ever need this again: since the template parameter T could be a pointer or a class, we can't just try and dereference it from
 * within this class by using '.' - we need to be able to use '->' if required. After much headache and faffing around, StackOverflow prevailed and
 * there are helper specialisations defined statically within the .tcc file. Use deref_if_pointer().blah() when calling functions on objects of type T.
 */

// TODO: Change the min dimensions/min object values to be passed in in the constructor - we don't really need
// to force them as template parameters, especially as it would be more useful to store the min dimension
// as a float and not an int.

template<typename T>
/**
 * @brief Specifies a tree node to be used for containing 3D geometry.
 * @note The type T stored MUST implement NGeometry::IConstBBoxVolume.
 */
class WorldCullTreeNode : public TreeNode
{
public:  
    /**
     * @brief Constructor.
     * @param bounds Bounding box representing this node's volume.
     * @param minDimension Minimum dimension for this node to be considered for splitting.
     * @param minObjects Min number of objects this node should contain if it is to be considered for splitting.
     * @param parent Parent node.
     */
    explicit WorldCullTreeNode(QBox3D bounds, float minDimension = MIN_DIMENSION, int minObjects = MIN_OBJECTS, WorldCullTreeNode* parent = NULL);

    /**
     * @brief Constructor.
     * @param bounds Bounding box representing this node's volume.
     * @param parent Parent node.
     */
    explicit WorldCullTreeNode(QBox3D bounds, WorldCullTreeNode* parent);

    /**
     * @brief Destructor.
     */
    virtual ~WorldCullTreeNode();

    /**
     * @brief Convenience function - returns the minimum length this node's sides must be for it to be
     * considered for recursive splitting.
     * @return Min length of each side of the octree node to split.
     */
    float splitMinDimensions() const  { return m_flMinDimension; }
        
    /**
     * @brief Convenience function - returns the minimum number of objects inside the node for it to be
     * considered for recursive splitting.
     * @return Min number of objects in the node to split.
     */
    int splitMinObjects() const     { return m_iMinObjects; }

    /**
     * @brief Adds an object to this node. If the object exists it is overwritten.
     * @param obj Object to add.
     */
    void addObject(const T &obj);
        
    /**
     * @brief Adds an object to this node recursively.
     * 
     * The object is only considered for adding if its bounding box is not completely disjoint from the bounding box of the node.
     * If the node is a leaf, the object is added to it; if not, the function is called recursively on each of the node's children.
     * @param obj Object to add.
     */
    void addObjectRecurse(const T &obj);
        
    /**
     * @brief Removes the given object from the node, if it exists.
     * @param obj Object to remove.
     */
    void removeObject(const T &obj);
    
    /**
     * @brief Removes the given object from the node and all of its children.
     * @param obj Object to remove.
     */
    void removeObjectRecurse(const T &obj);
    
    /**
     * @brief Removes all objects from this node only.
     */
    void removeAllObjects();
    
    /**
     * @brief Removes all objects from this node and all of its children.
     */
    void removeAllObjectsRecurse();
    
    /**
     * @brief Updates the object's presence in this node. It is added or removed as
     * applicable depending on its bounding box.
     * @param obj Object to update.
     */
    void updateObject(const T &obj);
    
    /**
     * @brief Updates the object's presence in this node and all of its children.
     * @param obj Object to update.
     */
    void updateObjectRecurse(const T &obj);
    
    /**
     * @brief Recursively checks object lists in leaf nodes, removing object references
     * if the object's bounding box no longer intersects this node.
     * @note This function does not have the scope to add object references to nodes where
     * it doesn't already exist.
     */
    void updateAllObjectsRecurse();
    bool containsObject(const T &obj) const;
    WorldCullTreeNode<T>* findObjectRecurse(const T &obj);
    int objectCount() const;
    
    typename QHash<T,char>::const_iterator objectsConstBegin() const;
    typename QHash<T,char>::const_iterator objectsConstEnd() const;
    
    typename QHash<T,char>::iterator objectsBegin();
    typename QHash<T,char>::iterator objectsEnd();
    
    QBox3D bounds() const;
    void setBounds(QBox3D bounds);
        
    void splitRecurse();
        
    // Convenience overrides.
    virtual void addChild(WorldCullTreeNode<T>* node)         { TreeNode::addChild(node); }
    virtual WorldCullTreeNode<T>* removeChild(int index)      { return dynamic_cast<WorldCullTreeNode<T>*>(TreeNode::removeChild(index)); }
    virtual WorldCullTreeNode<T>* childAt(int index) const    { return dynamic_cast<WorldCullTreeNode<T>*>(TreeNode::childAt(index)); }
    virtual WorldCullTreeNode<T>* parent() const              { return dynamic_cast<WorldCullTreeNode<T>*>(TreeNode::parent()); }
    virtual void setParent(WorldCullTreeNode<T>* parent)      { TreeNode::setParent(parent); }
   
private:
    bool checkImplementsInterfaces(const T &obj) const;
    QBox3D          m_Bounds;           // Bounding box for this node.
    QHash<T, char>  m_ObjectHash;       // Hash table of objects this node stores.
    float           m_flMinDimension;
    int             m_iMinObjects;
};

DATASTRUCTURES_END_NAMESPACE

#include "worldculltreenode.tcc"

#endif // WORLDCULLTREENODE_H
