#ifndef WORLDCULLTREENODE_H
#define WORLDCULLTREENODE_H

#include "octree_global.h"
#include "treenode.h"
#include <QHash>
#include <Qt3D/QBox3D>

OCTREE_BEGIN_NAMESPACE

/* The saga of this class: http://stackoverflow.com/questions/22783851/c-calling-derived-specialised-virtual-functions-within-a-base-template-class/
 * For reference if I ever need this again: since the template parameter T could be a pointer or a class, we can't just try and dereference it from
 * within this class by using '.' - we need to be able to use -> if required. After much headache and faffing around StackOverflow prevailed and
 * there are helper specialisations defined statically within the .tcc file. Use deref_if_pointer().blah() when calling functions on objects of
 * type T.
 */

template<typename T, int MD, int MO>
/**
 * @brief Specifies a tree node to be used for containing 3D geometry.
 * @note The type T stored MUST implement NGeometry::IConstBBoxVolume.
 */
class WorldCullTreeNodeAbstract : public TreeNode
{
public:
    explicit WorldCullTreeNodeAbstract(QBox3D bounds, WorldCullTreeNodeAbstract* parent = NULL);
    virtual ~WorldCullTreeNodeAbstract();

    int splitMinDimensions() const  { return MD; }
    int splitMinObjects() const     { return MO; }

    void addObject(const T &obj);
    void addObjectRecurse(const T &obj);
    void removeObject(const T &obj);
    void removeObjectRecurse(const T &obj);
    void removeAllObjects();
    void removeAllObjectsRecurse();
    void updateObject(const T &obj);
    void updateObjectRecurse(const T &obj);
    void updateAllObjectsRecurse();
    bool containsObject(const T &obj) const;
    WorldCullTreeNodeAbstract<T,MD,MO>* findObjectRecurse(const T &obj);
    int objectCount() const;
    
    typename QHash<T,char>::const_iterator objectsConstBegin() const;
    typename QHash<T,char>::const_iterator objectsConstEnd() const;
    
    typename QHash<T,char>::iterator objectsBegin();
    typename QHash<T,char>::iterator objectsEnd();
    
    QBox3D bounds() const;
    void setBounds(QBox3D bounds);
   
private:
    bool checkImplementsInterfaces(const T &obj) const;
    QBox3D          m_Bounds;       // Bounding box for this node.
    
protected:
    QHash<T, char>  m_ObjectHash;   // Hash table of objects this node stores.
};

OCTREE_END_NAMESPACE

#include "worldculltreenode.tcc"

#endif // WORLDCULLTREENODE_H
