#ifndef WORLDCULLTREENODE_H
#define WORLDCULLTREENODE_H

#include "octree_global.h"
#include "treenode.h"
#include <QHash>
#include <Qt3D/QBox3D>

OCTREE_BEGIN_NAMESPACE

/* OK, it's half 1 in the morning and I'm tired, so I'l write a quick note on what the hell is going on in this file.
 * 
 * WorldCullTreeNodeAbstract (which is no longer abstract) is a template class that should allow objects with bounding
 * boxes to be put into an octree. However, the method for calling functions on the object ('.' vs '->') differs
 * depending on whether the T parameter is a pointer or not. Hence, the way I ended up getting it to "work" (if that
 * actually holds up into the future) was by having WorldCullTreeNodeAbstract call virtual protected functions at the
 * points which it would previously have called functions on the T object. I then derived a partially specialised child
 * class which overrides these functions to implement the dereferencing correctly depending on whether T is a pointer
 * or not. Oddly enough, after having done this the syntax turned out to be identical ('->') in both specialised templates.
 * 
 * After a short test this code doesn't -seem- to throw up any more errors, although I am currently getting unresolved
 * externals relating to boundingBoxesIntersect() from the Geometry module, so I'm praying that's something different.
 * I just hope I never have to touch this file again...
 * 
 * - x6herbius
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
    virtual void addObjectRecurse(const T &obj);
    void removeObject(const T &obj);
    void removeObjectRecurse(const T &obj);
    void removeAllObjects();
    void removeAllObjectsRecurse();
    virtual void updateObject(const T &obj);
    void updateObjectRecurse(const T &obj);
    virtual void updateAllObjectsRecurse();
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
    virtual bool checkImplementsInterfaces(const T &obj) const;
    
    QBox3D          m_Bounds;       // Bounding box for this node.
    
protected:
    virtual QBox3D specialisedBoundsForType(const T* obj) const;        // Override this in derives specialisations.
    virtual bool specialisedImplementationCheck(const T* obj) const;    // Override this in derives specialisations.
    QHash<T, char>  m_ObjectHash;                                       // Hash table of objects this node stores.
};

// Specialised for objects.
template<typename T, int MD, int MO>
class WorldCullTreeNode : public WorldCullTreeNodeAbstract<T,MD,MO>
{
public:
    explicit WorldCullTreeNode(QBox3D bounds, WorldCullTreeNode* parent = NULL) : WorldCullTreeNodeAbstract(bounds, parent) {}
    virtual ~WorldCullTreeNode() {}
    
protected:
    virtual QBox3D specialisedBoundsForType(const T *obj) const
    {
        return obj->boundingBox();
    }
    
    virtual bool specialisedImplementationCheck(const T *obj) const
    {
        return obj->_implementsIConstBBoxVolume();
    }
};

// Specialised for objects.
template<typename T, int MD, int MO>
class WorldCullTreeNode<T*,MD,MO> : public WorldCullTreeNodeAbstract<T*,MD,MO>
{
public:
    explicit WorldCullTreeNode(QBox3D bounds, WorldCullTreeNode* parent = NULL) : WorldCullTreeNodeAbstract(bounds, parent) {}
    virtual ~WorldCullTreeNode() {}
    
protected:
    virtual QBox3D specialisedBoundsForType(const T *obj) const
    {
        return obj->boundingBox();
    }
    
    virtual bool specialisedImplementationCheck(const T *obj) const
    {
        return obj->_implementsIConstBBoxVolume();
    }
};

OCTREE_END_NAMESPACE

#include "worldculltreenode.tcc"

#endif // WORLDCULLTREENODE_H
