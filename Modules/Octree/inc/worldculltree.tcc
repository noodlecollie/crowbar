#include "worldculltree.h"
#include "array2d.h"
#include <QVector3D>

OCTREE_BEGIN_NAMESPACE

template< typename T, int AS >
WorldCullTree<T,AS>::WorldCullTree(int size, int magnitude) :
    m_iMagnitude(magnitude), m_Octree(size, NULL), m_HashTable(), m_NodeHashTable()
{
}

template< typename T, int AS >
WorldCullTree<T,AS>::WorldCullTree(const WorldCullTree<T,AS>& o) :
    m_iMagnitude(o.m_iMagnitude), m_Octree(o.m_Octree), m_HashTable(o.m_HashTable), m_NodeHashTable(o.m_NodeHashTable)
{
}

template< typename T, int AS >
WorldCullTree<T,AS>::~WorldCullTree()
{
    // Delete all dynamically allocated NodeHashes that are still present in the octree.
    // We now use a QHash to record all allocated NodeHashes, because iterating over every
    // node in the octree here is really slow.
    QHash<NodeHash*, char>::const_iterator it = m_NodeHashTable.constBegin();
    while ( it != m_NodeHashTable.constEnd() )
    {
        NodeHash* h = it.key();
        if ( h ) delete h;
        
        it++;
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
QBox3D WorldCullTree<T,AS>::nodeBbox(const QVector3D &pos) const
{
    // Translate position to an octree index.
    OctreeIndex i;
    vector3DToOctreeIndex(pos, i);
    
    // Translate octree index to bounding box.
    return octreeIndexToBbox(i);
}

template< typename T, int AS >
QBox3D WorldCullTree<T,AS>::octreeIndexToBbox(const OctreeIndex &index) const
{
    // Given an octree node index, get the world co-ordinate of the index (ie. the centre of the node).
    QVector3D centre = octreeIndexToVector3D(index);
    
    // The minimum point is centre - (0.5 * leafNodeDimension()) in each axis, and the maximum point is centre + the same.
    float deltaComponent = 0.5f * leafNodeDimension();
    QVector3D delta(deltaComponent, deltaComponent, deltaComponent);
    return QBox3D(centre - delta, centre + delta);
}

template< typename T, int AS >
QVector3D WorldCullTree<T,AS>::octreeIndexToVector3D(const OctreeIndex &index) const
{
    return QVector3D(indexToAxisValue(index.x),indexToAxisValue(index.y),indexToAxisValue(index.z));
}

template< typename T, int AS >
float WorldCullTree<T,AS>::indexToAxisValue(int index) const
{
    // Can you tell my commenting is basically me thinking aloud as I code up a solution? :P
    
    // If an index ordinate is less between 0 and (octreeSize()/2)-1, it corresponds to a negative world ordinate.
    // If it's between octreeSize()/2 and octreeSize()-1 it corresponds to a positive world ordinate.
    // It would be helpful if we could first establish an imaginary index which would represent a world ordinate of 0.
    // (This index is "imaginary" since it's a fraction.)
    float iOrigin = ((float)octreeSize()/2.0f) - 0.5f;
    
    // Any ordinate index - iOrigin will give the distance from the octree origin in this axis: positive or negative depending on the direction.
    // It would also help if we defined a "local magnitude" - half the size of the octree.
    float localMag = octreeSize() / 2.0f;
    
    // Now (index - iOrigin)/localMag will give the fraction of the magnitude from the origin in which the point lies in a given dimension.
    // We multiply our world magnitude by this fraction in order to get the real-world position in this axis.
    return (float)magnitude() * ( ((float)index-iOrigin)/localMag );
}

template< typename T, int AS >
float WorldCullTree<T,AS>::leafNodeDimension() const
{
    // Length of an edge of a leaf node is 2*magnitude divided by the number of leaf nodes in a given axis.
    return (2.0f*(float)magnitude()) / (float)octreeSize();
}

template< typename T, int AS >
typename WorldCullTree<T,AS>::NodeHash::const_iterator WorldCullTree<T,AS>::contents(const QVector3D &pos, bool &success) const
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
typename WorldCullTree<T,AS>::NodeHash::const_iterator WorldCullTree<T,AS>::constEnd(const QVector3D &pos, bool &success) const
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
void WorldCullTree<T,AS>::box3dToOctreeRange(const QBox3D &box, OctreeRange &range) const
{
    range.max_x = m_Octree.mapToNodeIndex(m_iMagnitude, box.maximum().x());
    range.max_y = m_Octree.mapToNodeIndex(m_iMagnitude, box.maximum().y());
    range.max_z = m_Octree.mapToNodeIndex(m_iMagnitude, box.maximum().z());
    range.min_x = m_Octree.mapToNodeIndex(m_iMagnitude, box.minimum().x());
    range.min_y = m_Octree.mapToNodeIndex(m_iMagnitude, box.minimum().y());
    range.min_z = m_Octree.mapToNodeIndex(m_iMagnitude, box.minimum().z());
}

template< typename T, int AS >
void WorldCullTree<T,AS>::vector3DToOctreeIndex(const QVector3D &pos, OctreeIndex &index) const
{
    index.x = m_Octree.mapToNodeIndex(magnitude(), pos.x());
    index.y = m_Octree.mapToNodeIndex(magnitude(), pos.y());
    index.z = m_Octree.mapToNodeIndex(magnitude(), pos.z());
}

template< typename T, int AS >
typename WorldCullTree<T,AS>::NodeHash* WorldCullTree<T,AS>::at(const OctreeIndex &index) const
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
                    m_NodeHashTable.insert(h, 0);   // Record the pointer so we can delete it later.
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
                    m_NodeHashTable.remove(h);
                    m_Octree.set(x, y, z, NULL);
                }
            }
        }
    }
}

template< typename T, int AS >
typename WorldCullTree<T,AS>::FrustumEnclose WorldCullTree<T,AS>::bboxTest(const QBox3D &bbox, const NGeometry::Frustum &frustum)
{
    // The following rules apply when testing a bounding box for presence inside a frustum:
    // - If min or max is not in front of a plane, it is considered to be behind the plane.
    // - Both min and max are behind all planes if and only if the box is completely inside the frustum.
    // - Both min and max are in front of the same plane if and only if the box is completely outside the frustum.
    // - If a box encloses the frustum in any way, it is partially enclosed by the frustum.
    
    // Set up a loop to test each plane in turn.
    const QPlane3D* planes = reinterpret_cast<const QPlane3D*>(&frustum);
    bool behindAllPlanesSoFar = true;
    for ( int i = 0; i < 6; i++ )
    {
        // Test min and max against the plane.
        qreal minDist = planes[i].distance(bbox.minimum());
        qreal maxDist = planes[i].distance(bbox.maximum());
        
        // If both are positive, we immediately know that the bounding box is outside the volume.
        if ( minDist > 0.0 && maxDist > 0.0 )
        {
            return FENone;
        }
        
        // If one or the other is in front, unset our behindAllPlanes flag.
        if ( minDist > 0.0 || maxDist > 0.0 )
        {
            behindAllPlanesSoFar = false;
        }
    }
    
    // If behindAllPlanesSoFar is true after all plane checks, we are fully within the volume.
    // Otherwise we are partially inside.
    return behindAllPlanesSoFar ? FEFull : FEPartial;
}

OCTREE_END_NAMESPACE
