#ifndef WORLDCULLTREE_H
#define WORLDCULLTREE_H

#include "octree_global.h"

#include "octree.h"
#include <QHash>
#include <Qt3D/QBox3D>

OCTREE_BEGIN_NAMESPACE

template< typename T, int AS = 1 >
class WorldCullTree
{
public:
    /**
     * @brief Constructor.
     * @param size Number of nodes the tree contains in each axis (from -magnitude to +magnitude).
     * @param magnitude Distance from centre of octree to furthest point in each axis.
     * @param emptyValue Value that will be returned if a node at a given position does not exist.
     */
    WorldCullTree( int size, int magnitude, const T& emptyValue = T(0) );
    WorldCullTree( const WorldCullTree<T,AS>& o );
    virtual ~WorldCullTree();
    
    int magnitude() const;
    int size() const;
    const T& emptyValue() const;
    void setEmptyValue( const T& emptyValue );
    
private:
    int                 m_iMagnitude;   // Distance from centre of octree to furthest point in each axis.
    Octree<T,AS>        m_Octree;       // Pointer to octree this class contains.
    QHash<T*, QBox3D>    m_HashTable;   // Hash table of objects and bounding boxes.
};

OCTREE_END_NAMESPACE

#include "worldculltree.tcc"

#endif // WORLDCULLTREE_H
