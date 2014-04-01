#include "worldculltreenode.h"
#include "iconstbboxvolume.h"
#include "geomutil.h"
#include <QList>

OCTREE_BEGIN_NAMESPACE

template<typename T, int MD, int MO>
WorldCullTreeNodeAbstract<T,MD,MO>::WorldCullTreeNodeAbstract(QBox3D bounds, WorldCullTreeNodeAbstract* parent) : TreeNode(parent),
    m_ObjectHash(), m_Bounds(bounds)
{
}

template<typename T, int MD, int MO>
WorldCullTreeNodeAbstract<T,MD,MO>::~WorldCullTreeNodeAbstract()
{
}

template<typename T, int MD, int MO>
QBox3D WorldCullTreeNodeAbstract<T,MD,MO>::bounds() const
{
    return m_Bounds;
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::setBounds(QBox3D bounds)
{
    m_Bounds = bounds;
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::addObject(const T &obj)
{
    // Ensure we implement the required interfaces.
    Q_ASSERT( checkImplementsInterfaces(obj) );
    
    // Record the object.
    m_ObjectHash.insert(obj, 0);    // Char used for memory conservation.
}

template<typename T, int MD, int MO>
QBox3D WorldCullTreeNodeAbstract<T,MD,MO>::specialisedBoundsForType(const T *obj) const
{
    Q_UNUSED(obj);
    
    // We shouldn't be here!
    Q_ASSERT( false );
    return QBox3D();
}

template<typename T, int MD, int MO>
bool WorldCullTreeNodeAbstract<T,MD,MO>::specialisedImplementationCheck(const T *obj) const
{
    Q_UNUSED(obj);
    
    // We shouldn't be here!
    Q_ASSERT( false );
    return false;
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::addObjectRecurse(const T &obj)
{
    // Ensure we implement the required interfaces.
    Q_ASSERT( checkImplementsInterfaces(obj) );
    
    // If the bounding box intersects ours, add to this node (or its children).
    if ( NGeometry::boundingBoxesIntersect(bounds(), specialisedBoundsForType(&obj)) )
    {
        // If we're not a leaf, add to our children.
        if ( !isLeaf() )
        {
            for ( int i = 0; i < childCount(); i++ )
            {
                WorldCullTreeNodeAbstract* child = dynamic_cast<WorldCullTreeNodeAbstract*>(childAt(i));
                Q_ASSERT( child );
                child->addObjectRecurse(obj);
            }
        }
        
        // Otherwise, add to this node.
        else
        {
            addObject(obj);
        }
    }
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::removeObject(const T &obj)
{
    // Ensure we implement the required interfaces.
    Q_ASSERT( checkImplementsInterfaces(obj) );
    
    // Remove the object.
    m_ObjectHash.remove(obj);
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::removeObjectRecurse(const T &obj)
{
    // Ensure we implement the required interfaces.
    Q_ASSERT( checkImplementsInterfaces(obj) );
    
    // Attempt removal from ourself.
    removeObject(obj);
    
    // Remove from all children too.
    for ( int i = 0; i < childCount(); i++ )
    {
        WorldCullTreeNodeAbstract* child = dynamic_cast<WorldCullTreeNodeAbstract*>(childAt(i));
        Q_ASSERT( child );
        child->removeObject(obj);
    }
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::removeAllObjects()
{
    m_ObjectHash.clear();
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::removeAllObjectsRecurse()
{
    // Remove our objects.
    removeAllObjects();
    
    // Remove all objects from our children.
    for ( int i = 0; i < childCount(); i++ )
    {
        WorldCullTreeNodeAbstract* child = dynamic_cast<WorldCullTreeNodeAbstract*>(childAt(i));
        Q_ASSERT( child );
        child->removeAllObjectsRecurse();
    }
}

template<typename T, int MD, int MO>
bool WorldCullTreeNodeAbstract<T,MD,MO>::containsObject(const T &obj) const
{
    // Ensure we implement the required interfaces.
    Q_ASSERT( checkImplementsInterfaces(obj) );
    
    return m_ObjectHash.contains(obj);
}

template<typename T, int MD, int MO>
WorldCullTreeNodeAbstract<T,MD,MO>* WorldCullTreeNodeAbstract<T,MD,MO>::findObjectRecurse(const T &obj)
{
    // Ensure we implement the required interfaces.
    Q_ASSERT( checkImplementsInterfaces(obj) );
    
    // If the object is present in this node, return it.
    if ( containsObject(obj) ) return this;
    
    // Check recursively through children to see if the object is there.
    for ( int i = 0; i < childCount(); i++ )
    {
        WorldCullTreeNodeAbstract* child = dynamic_cast<WorldCullTreeNodeAbstract*>(childAt(i));
        Q_ASSERT( child );
        
        WorldCullTreeNodeAbstract* found = child->findObjectRecurse(obj);
        if ( found ) return found; // Return the node if it holds the object.
    }
    
    return NULL;    // Return NULL otherwise.
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::updateObject(const T &obj)
{
    // Ensure we implement the required interfaces.
    Q_ASSERT( checkImplementsInterfaces(obj) );
    
    // If the bounding box intersects ours, add.
    if ( NGeometry::boundingBoxesIntersect(bounds(), specialisedBoundsForType(&obj)) )
    {
        addObject(obj);
    }
    
    // If not, remove.
    else
    {
        removeObject(obj);
    }
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::updateObjectRecurse(const T &obj)
{
    // Ensure we implement the required interfaces.
    Q_ASSERT( checkImplementsInterfaces(obj) );
    
    // If we have children, update in them first.
    if ( !isLeaf() )
    {
        for ( int i = 0; i < childCount(); i++ )
        {
            WorldCullTreeNodeAbstract* child = dynamic_cast<WorldCullTreeNodeAbstract*>(childAt(i));
            Q_ASSERT( child );
            child->updateObjectRecurse(obj);
        }
    }
    
    // Otherwise, update in this node.
    else
    {
        updateObject(obj);
    }
}

template<typename T, int MD, int MO>
void WorldCullTreeNodeAbstract<T,MD,MO>::updateAllObjectsRecurse()
{
    // If we have children, update in them first.
    if ( !isLeaf() )
    {
        for ( int i = 0; i < childCount(); i++ )
        {
            WorldCullTreeNodeAbstract* child = dynamic_cast<WorldCullTreeNodeAbstract*>(childAt(i));
            Q_ASSERT( child );
            child->updateAllObjectsRecurse();
        }
    }
    
    // Otherwise, update in this node.
    else
    {
        // I don't think we can remove objects live when iterating, so make a list.
        QList<T> toRemove;
        
        // Discover objects to remove.
        // Whoops, foreach returns the values from the hash, not the keys...
//        foreach( T obj, m_ObjectHash )
//        {
//            if ( NGeometry::boundingBoxesIntersect(bounds(), specialisedBoundsForType(&obj)) )
//            {
//                toRemove.append(obj);
//            }
//        }
        
        QHash<T,char>::const_iterator it = m_ObjectHash.constBegin();
        
        while ( it != m_ObjectHash.constEnd() )
        {
            T obj = it.key();
            
            if ( NGeometry::boundingBoxesIntersect(bounds(), specialisedBoundsForType(&obj)) )
            {
                toRemove.append(obj);
            }
        }
        
        // Remove them.
        foreach( T obj, toRemove )
        {
            removeObject(obj);
        }
    }
}

template<typename T, int MD, int MO>
int WorldCullTreeNodeAbstract<T,MD,MO>::objectCount() const
{
    return m_ObjectHash.size();
}

template<typename T, int MD, int MO>
bool WorldCullTreeNodeAbstract<T,MD,MO>::checkImplementsInterfaces(const T &obj) const
{
    // This -should- cause a compilation error if the method is not implemented.
    return specialisedImplementationCheck(&obj);
}

template<typename T, int MD, int MO>
typename QHash<T,char>::const_iterator WorldCullTreeNodeAbstract<T,MD,MO>::objectsConstBegin() const
{
    return m_ObjectHash.constBegin();
}

template<typename T, int MD, int MO>
typename QHash<T,char>::const_iterator WorldCullTreeNodeAbstract<T,MD,MO>::objectsConstEnd() const
{
    return m_ObjectHash.constEnd();
}

template<typename T, int MD, int MO>
typename QHash<T,char>::iterator WorldCullTreeNodeAbstract<T,MD,MO>::objectsBegin()
{
    return m_ObjectHash.begin();
}

template<typename T, int MD, int MO>
typename QHash<T,char>::iterator WorldCullTreeNodeAbstract<T,MD,MO>::objectsEnd()
{
    return m_ObjectHash.end();
}

OCTREE_END_NAMESPACE
