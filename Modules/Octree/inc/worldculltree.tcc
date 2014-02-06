#include "worldculltree.h"
#include "array2d.h"

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
