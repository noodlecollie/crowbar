#include "worldculltree.h"
#include "array2d.h"
#include <QVector3D>

OCTREE_BEGIN_NAMESPACE

template< typename T, int AS >
WorldCullTree<T,AS>::WorldCullTree(int size, int magnitude) :
    m_iMagnitude(magnitude), m_Octree(size, NULL), m_HashTable()
{
}

template< typename T, int AS >
WorldCullTree<T,AS>::WorldCullTree(const WorldCullTree<T,AS>& o) :
    m_iMagnitude(o.m_iMagnitude), m_Octree(o.m_Octree), m_HashTable(o.m_HashTable)
{
}

template< typename T, int AS >
WorldCullTree<T,AS>::~WorldCullTree()
{
    // Delete all dynamically allocated NodeHashes that are still present in the octree.
    for ( int z = 0; z < m_Octree.size(); z++ )
    {
        Array2D<NodeHash*> tmp = m_Octree.zSlice(z);
        
        for ( int y = 0; y < m_Octree.size(); y++ )
        {
            for ( int x = 0; x < m_Octree.size(); x++ )
            {
                NodeHash* h = tmp(x, y);
                
                if ( h )
                {
                    delete h;
                    m_Octree.set(x, y, z, NULL);
                }
            }
        }
    }
}

template< typename T, int AS >
int WorldCullTree<T,AS>::magnitude() const
{
    return m_iMagnitude;
}

template< typename T, int AS >
int WorldCullTree<T,AS>::octreeSize() const
{
    return m_Octree.size();
}

template< typename T, int AS >
int WorldCullTree<T,AS>::count() const
{
    return m_HashTable.size();
}

template< typename T, int AS >
bool WorldCullTree<T,AS>::exists(const T obj) const
{
    return m_HashTable.contains(obj);
}

template< typename T, int AS >
QBox3D WorldCullTree<T,AS>::bboxFor(const T obj) const
{
    return m_HashTable.value(obj, QBox3D(QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0)));
}

template< typename T, int AS >
void WorldCullTree<T,AS>::add(const T obj, const QBox3D &bb)
{
    // Check whether object already exists in hash table.
    if ( m_HashTable.contains(obj) )
    {
        // If the bounding box is exactly the same, we don't need to do anything.
        QBox3D localbb = m_HashTable.value(obj);
        if ( localbb == bb ) return;
        
        // The bounding box is different, so the object should be replaced.
        // Remove it from the nodes it currently resides in.
        OctreeRange r;
        box3dToOctreeRange(localbb, r);
        removeReferenceFromTree(obj, r);
    }
    
    // Convert bounding box into octree co-ords.
    OctreeRange range;
    box3dToOctreeRange(bb, range);
    
    // Write object to octree.
    addReferenceToTree(obj, range);
    
    // Write object to hash table.
    // If it already exists it will be overwritten.
    m_HashTable.insert(obj, bb);
}

template< typename T, int AS >
void WorldCullTree<T,AS>::remove(const T obj)
{
    // Check whether object already exists in hash table.
    if ( !m_HashTable.contains(obj) ) return;
    
    // Remove the object from the octree first.
    OctreeRange range;
    box3dToOctreeRange(m_HashTable.value(obj), range);
    removeReferenceFromTree(obj, range);
    
    // Then remove the object from the hash table.
    m_HashTable.remove(obj);
}

template< typename T, int AS >
QHash::const_iterator<const T, char> WorldCullTree<T,AS>::contents(const QVector3D &pos, bool &success) const
{
    // Translate the position to an octree node.
    OctreeIndex i;
    vector3DToOctreeIndex(pos, i);
    
    // Get the hash table from the node.
    NodeHash* h = at(index);
    if ( !h )
    {
        // There was no hash table, return an invalid iterator.
        success = false;
        return QHash::const_iterator<const T, char>();
    }
    
    // Return the beginning iterator.
    success = true;
    return h->constBegin();
}

template< typename T, int AS >
QHash::const_iterator<const T, char> WorldCullTree<T,AS>::constEnd(const QVector3D &pos, bool &success) const
{
    // Translate the position to an octree node.
    OctreeIndex i;
    vector3DToOctreeIndex(pos, i);
    
    // Get the hash table from the node.
    NodeHash* h = at(index);
    if ( !h )
    {
        // There was no hash table, return an invalid iterator.
        success = false;
        return QHash::const_iterator<const T, char>();
    }
    
    // Return the end iterator.
    success = true;
    return h->constEnd();
}

template< typename T, int AS >
bool WorldCullTree<T,AS>::nodeEmpty(const QVector3D &pos) const
{
    OctreeIndex i;
    vector3DToOctreeIndex(pos, i);
    return !at(index) || at(index)->size() < 1;
}

template< typename T, int AS >
void WorldCullTree<T,AS>::contents(const QVector3D &pos, QList<const T> &list) const
{
    // Get the iterator for the contents of the corresponding node.
    bool b = false;
    
    QHash::const_iterator<const T, char> it = contents(pos, b);
    if ( !b ) return;
    
    QHash::const_iterator<const T, char> end = constEnd(pos, b);
    if ( !b ) return;
    
    // Add each of the elements to our output list.
    for ( ; it != end; ++it )
    {
        list.append(it.key());
    }
}

template< typename T, int AS >
void WorldCullTree<T,AS>::contents(const QBox3D &box, QList<const T> &list) const
{
    // Really we want a distinct list, so we'll add to a hash first and then
    // export to the list.
    NodeHash hash;
    
    // Convert box to octree range.
    OctreeRange range;
    box3dToOctreeRange(box, range);
    
    // Search each of the octree nodes.
    for ( int z = range.min_z; z <= range.max_z; z++ )
    {
        Array2D<NodeHash*> tmp = m_Octree.zSlice(z);
        
        for ( int y = range.min_y; y <= range.max_y; y++ )
        {
            for ( int x = range.min_x; x <= range.max_x; x++ )
            {
                // Get the hash table pointer.
                NodeHash* h = tmp(x, y);
                if ( !h ) continue;
                
                for ( QHash::const_iterator<const T, char> i = h->constBegin(); i != h->constEnd(); ++i )
                {
                    hash.insert(i.key(), i.value());
                }
            }
        }
    }
    
    // Now copy the distinct items to the list.
    for ( QHash::const_iterator<const T, char> i = hash.constBegin(); i != hash.constEnd(); ++i )
    {
        list.append(i.key());
    }
}

template< typename T, int AS >
void WorldCullTree<T,AS>::box3dToOctreeRange(const QBox3D &box, OctreeRange &range)
{
    range.max_x = m_Octree.mapToNodeIndex(m_iMagnitude, box.maximum().x());
    range.max_y = m_Octree.mapToNodeIndex(m_iMagnitude, box.maximum().y());
    range.max_z = m_Octree.mapToNodeIndex(m_iMagnitude, box.maximum().z());
    range.min_x = m_Octree.mapToNodeIndex(m_iMagnitude, box.minimum().x());
    range.min_y = m_Octree.mapToNodeIndex(m_iMagnitude, box.minimum().y());
    range.min_z = m_Octree.mapToNodeIndex(m_iMagnitude, box.minimum().z());
}

template< typename T, int AS >
void WorldCullTree<T,AS>::vector3DToOctreeIndex(const QVector3D &pos, OctreeIndex &index)
{
    index.x = m_Octree.mapToNodeIndex(pos.x());
    index.y = m_Octree.mapToNodeIndex(pos.y());
    index.z = m_Octree.mapToNodeIndex(pos.z());
}

template< typename T, int AS >
NodeHash* WorldCullTree<T,AS>::at(const OctreeIndex &index)
{
    return m_Octree.at(index.x, index.y, index.z);
}

template< typename T, int AS >
void WorldCullTree<T,AS>::addReferenceToTree(const T obj, const OctreeRange &range)
{
    for ( int z = range.min_z; z <= range.max_z; z++ )
    {
        // zSlice copies the pointers from the nodes into a 2D array, so modifying the array
        // won't modify the actual octree (I don't think). We'll need to use set() for that.
        Array2D<NodeHash*> tmp = m_Octree.zSlice(z);
        
        for ( int y = range.min_y; y <= range.max_y; y++ )
        {
            for ( int x = range.min_x; x <= range.max_x; x++ )
            {
                // If we currently have no hash table at this node, create one.
                NodeHash* h = tmp(x, y);
                if ( !h )
                {
                    h = new NodeHash();
                    m_Octree.set(x, y, z, h);
                }
                
                // Add the reference to the hash.
                // If it already exists, it is overwritten - check this outside of this function first!
                h->insert(obj, 0);
            }
        }
    }
}

template< typename T, int AS >
void WorldCullTree<T,AS>::removeReferenceFromTree(const T obj, const OctreeRange &range)
{
    for ( int z = range.min_z; z <= range.max_z; z++ )
    {
        // zSlice copies the pointers from the nodes into a 2D array, so modifying the array
        // won't modify the actual octree (I don't think). We'll need to use set() for that.
        Array2D<NodeHash*> tmp = m_Octree.zSlice(z);
        
        for ( int y = range.min_y; y <= range.max_y; y++ )
        {
            for ( int x = range.min_x; x <= range.max_x; x++ )
            {
                // Check that we have a hash table first.
                NodeHash* h = tmp(x, y);
                if ( !h ) continue;
                
                // Remove the reference.
                h->remove(obj);
                
                // If the hash table is now empty, delete it.
                if ( h->size() < 1 )
                {
                    delete h;
                    m_Octree.set(x, y, z, NULL);
                }
            }
        }
    }
}

OCTREE_END_NAMESPACE
