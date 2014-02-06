#ifndef WORLDCULLTREE_H
#define WORLDCULLTREE_H

#include "octree_global.h"

#include "octree.h"
#include <QHash>
#include <Qt3D/QBox3D>
#include <QVector3D>

OCTREE_BEGIN_NAMESPACE

template< typename T, int AS = 1 >
class WorldCullTree
{
public:
    // Char is used solely to minimise memory - we only care that the key is present.
    typedef QHash<const T, char> NodeHash;
        
    /**
     * @brief Constructor.
     * @param size Number of nodes the tree contains in each axis (from -magnitude to +magnitude).
     * @param magnitude Distance from centre of octree to furthest point in each axis.
     */
    WorldCullTree( int size, int magnitude );
    WorldCullTree( const WorldCullTree<T,AS>& o );
    virtual ~WorldCullTree();
    
    int magnitude() const;
    int octreeSize() const;
    int count() const;
        
    void add(const T obj, const QBox3D &bb);
    void remove(const T obj);
    bool exists(const T obj);
    
private:
    struct OctreeRange
    {
        int min_x;
        int min_y;
        int min_z;
        int max_x;
        int max_y;
        int max_z;
    };
        
    void addReferenceToTree(const T obj, const OctreeRange &range);
    void removeReferenceFromTree(const T obj, const OctreeRange &range);
        
    int                     m_iMagnitude;   // Distance from centre of octree to furthest point in each axis.
    Octree<NodeHash*, AS>   m_Octree;       // Pointer to octree this class contains.
    QHash<T, QBox3D>        m_HashTable;    // Hash table of objects and bounding boxes.
};

OCTREE_END_NAMESPACE

#include "worldculltree.tcc"

#endif // WORLDCULLTREE_H
