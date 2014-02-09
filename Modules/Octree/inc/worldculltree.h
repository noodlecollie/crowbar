#ifndef WORLDCULLTREE_H
#define WORLDCULLTREE_H

#include "octree_global.h"

#include "octree.h"
#include <QHash>
#include <Qt3D/QBox3D>
#include <QList>
#include "frustum_util.h"

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
    bool exists(const T obj) const;
    bool nodeEmpty(const QVector3D &pos) const;
    QBox3D bboxFor(const T obj) const;
        
    // If there is nothing in the node, an invalid const_iterator is returned and
    // success is set to false.
    typename NodeHash::const_iterator contents(const QVector3D &pos, bool &success) const;
    typename NodeHash::const_iterator constEnd(const QVector3D &pos, bool &success) const;
        
    void contents(const QVector3D &pos, QList<const T> &list) const;
    void contents(const QBox3D &box, QList<const T> &list) const;
    
    // Make these private again once we're finished testing!!
    // To what extent a frustum volume encloses an object.
    enum FrustumEnclose
    {
        FEFull = 0, // Object is completely inside the frustum.
        FEPartial,  // Object is partially inside the frustum.
        FENone      // Object is completely outside the frustum.
    };
    static FrustumEnclose bboxTest(const QBox3D &bbox, const NGeometry::Frustum &frustum);
    // -----
    
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
        
    struct OctreeIndex
    {
        int x;
        int y;
        int z;
    };
        
    void addReferenceToTree(const T obj, const OctreeRange &range);
    void removeReferenceFromTree(const T obj, const OctreeRange &range);
    void box3dToOctreeRange(const QBox3D &box, OctreeRange &range) const;
    void vector3DToOctreeIndex(const QVector3D &pos, OctreeIndex &index) const;
    NodeHash* at(const OctreeIndex &index) const;
        
    int                     m_iMagnitude;   // Distance from centre of octree to furthest point in each axis.
    Octree<NodeHash*, AS>   m_Octree;       // Pointer to octree this class contains.
    QHash<const T, QBox3D>        m_HashTable;    // Hash table of objects and bounding boxes.
};

OCTREE_END_NAMESPACE

#include "worldculltree.tcc"

#endif // WORLDCULLTREE_H
