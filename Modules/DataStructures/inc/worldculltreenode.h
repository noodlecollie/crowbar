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

/* The saga of this class: http://stackoverflow.com/questions/22783851/c-calling-derived-specialised-virtual-functions-within-a-base-template-class/
 * For reference if I ever need this again: since the template parameter T could be a pointer or a class, we can't just try and dereference it from
 * within this class by using '.' - we need to be able to use '->' if required. After much headache and faffing around, StackOverflow prevailed and
 * there are helper specialisations defined statically within the .tcc file. Use deref_if_pointer().blah() when calling functions on objects of type T.
 */

template<typename T, int MD = 1024, int MO = 2>
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
     * @param parent
     */
    explicit WorldCullTreeNode(QBox3D bounds, WorldCullTreeNode* parent = NULL);

    /**
     * @brief Destructor.
     */
    virtual ~WorldCullTreeNode();

    /**
     * @brief Convenience function - returns the minimum length this node's sides must be for it to be
     * considered for recursive splitting.
     * @return Min length of each side of the octree node to split.
     */
    int splitMinDimensions() const  { return MD; }
        
    /**
     * @brief Convenience function - returns the minimum number of objects inside the node for it to be
     * considered for recursive splitting.
     * @return Min number of objects in the node to split.
     */
    int splitMinObjects() const     { return MO; }

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
    void removeObjectRecurse(const T &obj);
    void removeAllObjects();
    void removeAllObjectsRecurse();
    void updateObject(const T &obj);
    void updateObjectRecurse(const T &obj);
    void updateAllObjectsRecurse();
    bool containsObject(const T &obj) const;
    WorldCullTreeNode<T,MD,MO>* findObjectRecurse(const T &obj);
    int objectCount() const;
    
    typename QHash<T,char>::const_iterator objectsConstBegin() const;
    typename QHash<T,char>::const_iterator objectsConstEnd() const;
    
    typename QHash<T,char>::iterator objectsBegin();
    typename QHash<T,char>::iterator objectsEnd();
    
    QBox3D bounds() const;
    void setBounds(QBox3D bounds);
        
    void splitRecurse();
   
private:
    WorldCullTreeNode<T,MD,MO>* cullTreeChildAt(int index) const;   // Convenience
    bool checkImplementsInterfaces(const T &obj) const;
    QBox3D          m_Bounds;       // Bounding box for this node.
    QHash<T, char>  m_ObjectHash;   // Hash table of objects this node stores.
};

DATASTRUCTURES_END_NAMESPACE

#include "worldculltreenode.tcc"

#endif // WORLDCULLTREENODE_H
