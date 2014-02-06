#include "worldculltree.h"

OCTREE_BEGIN_NAMESPACE

template< typename T, int AS >
WorldCullTree<T,AS>::WorldCullTree(int size, int magnitude, const T &emptyValue) :
    m_iMagnitude(magnitude), m_Octree(size, emptyValue)
{
}

template< typename T, int AS >
WorldCullTree<T,AS>::WorldCullTree(const WorldCullTree<T,AS>& o) :
    m_iMagnitude(o.m_iMagnitude), m_Octree(o.m_Octree)
{
}

template< typename T, int AS >
WorldCullTree<T,AS>::~WorldCullTree()
{
}

template< typename T, int AS >
int WorldCullTree<T,AS>::magnitude() const
{
    return m_iMagnitude;
}

template< typename T, int AS >
int WorldCullTree<T,AS>::size() const
{
    return m_Octree.size();
}

template< typename T, int AS >
const T& WorldCullTree<T,AS>::emptyValue() const
{
    return m_Octree.emptyValue();
}

template< typename T, int AS >
void WorldCullTree<T,AS>::setEmptyValue(const T& emptyValue)
{
    m_Octree.setEmptyValue(emptyValue);
}

OCTREE_END_NAMESPACE
