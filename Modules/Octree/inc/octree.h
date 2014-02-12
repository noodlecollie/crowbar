/** \file octree.h
 * \brief Defines the octree class. Code (C) 2007  Simon Perreault. Modified by me to better suit my ends.
 */

/*
 *  Copyright (C) 2007  Simon Perreault
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OCTREE_H
#define OCTREE_H

#include "octree_global.h"

#include "array2d.h"
#include "point3d.h"

#include <algorithm>
#include <cassert>
#include <istream>
#include <ostream>
#include <QVector3D>

OCTREE_BEGIN_NAMESPACE

/**
 * \class Octree
 * \brief Generic octree template
 *
 * \author Simon Perreault <nomis80@nomis80.org>
 * \date April 2007
 *
 * This class template represents an octree, often used for manipulating 3-D
 * scattered data efficiently. The type of the contained data is supplied as a
 * template parameter.
 *
 * \param T Type of the contained data. Requirements on type: must be copyable
 * and default-constructible.
 *
 * \param AS Short for "aggregate size." As an optimization, leaves can be
 * aggregated so that the relative size of pointers is diminished. This is 1 by
 * default, but should be set higher when the size of \a T is small. <b>Must be
 * a power of two.</b>
 */
template< typename T, int AS = 1 >
class Octree
{
public:
    /**
     * \brief Constructor.
     * \param size Size of octree, in nodes. Should be a power of two. For example,
     * an octree with \a size = 256 will represent a cube divided into 256x256x256
     * nodes. <b>Must be a power of two.</b>
     *
     * \param emptyValue This is the value that will be returned when accessing
     * regions of the 3-D volume where no node has been allocated. In other words,
     * instead of following a null node pointer, this value is returned. Since the
     * octree root is initially a null pointer, the whole volume is initialized to
     * this value.
     */
    Octree( int size, const T& emptyValue = T(0) );
    
    /**
     * \brief Performs a deep copy of an octree. All branch pointers will be followed
     * recursively and new nodes will be allocated.
     *
     * \param o Octree to be copied.
     */
    Octree( const Octree<T,AS>& o );
    
    /**
     * \brief Recursively deletes all nodes by following branch pointers.
     */
    ~Octree();

    // Accessors
    /**
     * \brief Returns size of octree.
     * \return Size of octree, in nodes, as specified in the constructor.
     */
    int size() const;
    
    /**
     * \brief Returns the empty value this octree uses.
     * \return Value of empty nodes, as specified in the constructor.
     * \see setEmptyValue()
     */
    const T& emptyValue() const;

    /**
     * \brief Returns bytes occpuied by a branch node.
     * \return Number of bytes a branch node occupies.
     */
    static unsigned long branchBytes();
    
    /**
     * \brief Returns bytes occpuied by an aggregate node.
     * \return Number of bytes an aggregate node occupies.
     */
    static unsigned long aggregateBytes();
    
    /**
     * \brief Returns bytes occpuied by a leaf node.
     * \return Number of bytes a leaf node occupies.
     */
    static unsigned long leafBytes();
    
    /**
     * \brief Returns the number of bytes the octree occupies.
     * \return Total number of bytes the octree occupies.
     *
     * \remarks Memory fragmentation may make the actual memory usage significantly
     * higher.
     */
    unsigned long bytes() const;

    /**
     * Returns the number of nodes in the octree.
     * \return Total number of nodes in the octree.
     */
    int nodes() const;
    
    /**
     * \brief Returns the number of nodes at a given size.
     * \return Number of nodes at size \a size. For example, the root (if
     * allocated) is the single node of size 1. At size <i>n</i> there may be a
     * maximum of 2<sup><i>n</i></sup> nodes.
     *
     * For sizes lower than the aggregate size, this function will always return
     * zero.
     */
    int nodesAtSize( int size ) const;

    // Mutators
    /**
     * \brief Sets the empty value this octree uses.
     * \brief Sets the value of empty nodes to \a emptyValue.
     * \see setEmptyValue()
     */
    void setEmptyValue( const T& emptyValue );

    /**
     * \brief Swaps the octree's contents with another's. This is a cheap operation as only
     * the root pointers are swapped, not the whole structure.
     */
    void swap( Octree<T,AS>& o );
    
    /**
     * \brief Assigns to this octree the contents of octree \a o.
     */
    Octree<T,AS>& operator= ( Octree<T,AS> o );

    // Indexing operators
    /**
     * \brief Returns the value at a specific index.
     * \return Reference to value at index (\a x,\a y,\a z). If no node exists at
     * this index, a new one is created (along with the necessary ancestry),
     * initialized to the value returned by emptyValue(), and returned.
     *
     * \remarks Be careful when calling this function. If you do not want to
     * inadvertently create new nodes, use the at() function.
     *
     * \see at()
     */
    T& operator() ( int x, int y, int z );
    
    /**
     * \brief Synonym of at().
     */
    const T& operator() ( int x, int y, int z ) const;
    
    /**
     * \brief Returns the value at a specified index in the octree.
     * \return Value at index (\a x,\a y,\a z). If no node exists at this index, the
     * value returned by emptyValue() is returned.
     *
     * \remarks Memory access is faster when \a x varies the quickest, followed by
     * \a y and then by \a z. Therefore you should write nested loops in this order
     * for faster access:
     *
     * \code
     * for ( int z = 0; z < ...; ++z ) {
     *     for ( int y = 0; y < ...; ++y ) {
     *         for ( int x = 0; x < ...; ++x ) {
     *             ... = octree.at(x,y,z);
     *         }
     *     }
     * }
     * \endcode
     *
     * However, zSlice() provides an even faster way.
     */
    const T& at( int x, int y, int z ) const;

    /**
     * \brief Sets the value of the node at (\a x, \a y, \a z) to \a value. If \a value is
     * the empty value, the node is erased. Otherwise, the node is created if it did
     * not already exist and its value is set to \a value.
     */
    void set( int x, int y, int z, const T& value );
    
    /**
     * \brief Erases the node at index (\a x,\a y,\a z). After the call,
     * <code>at(x,y,z)</code> will return the value returned by emptyValue().
     *
     * This function will free as much memory as possible. For example, when erasing
     * the single child of a branch node, the branch node itself will be erased and
     * replaced by a null pointer in its parent. This will percolate to the top of
     * the tree if necessary.
     */
    void erase( int x, int y, int z );

    /**
     * \brief Returns a slice of the octree.
     * \return A slice of the octree, perpendicular to the Z axis. The content of
     * all nodes for which the Z index is \a z will be copied into the returned
     * array. If no node exists for a given index, the value returned by
     * emptyValue() will be written instead.
     *
     * \remarks This method ought to be relatively fast as long the the time
     * required to copy values does not dwarf the time for indexing into the octree
     * (this should be the case for built-in C++ types such as int and double).
     * As a result, using this function is an easy way to accelerate the infamous
     * three-level nested loops. For example:
     *
     * \code
     * for ( int z = 0; z < ...; ++z ) {
     *     tmp = octree.zSlice(z);
     *     for ( int y = 0; y < ...; ++y ) {
     *         for ( int x = 0; x < ...; ++x ) {
     *             ... = tmp(y,x);
     *         }
     *     }
     * }
     * \endcode
     */
    Array2D<T> zSlice( int z ) const;

    // I/O functions
    /**
     * \brief Writes the octree to an output stream.
     *
     * Writes the octree in binary form to the output stream \a out. This should be
     * fast, but note that the type \a T will be written as it appears in memory.
     * That is, if it is a complex type containing pointers, the pointer addresses
     * will be written instead of the data pointed at. For complex types, you should
     * roll your own function.
     */
    void writeBinary( std::ostream& out ) const;
    
    /**
     * \brief Reads the octree from \a in. It must previously have been written using
     * writeBinary().
     */
    void readBinary( std::istream& in );
        
    // ==================================
    // X6Herbius edits here:
        
    /**
     * @brief Given a total extent and the number of divisions within that extent,
     * remaps an input value to a 0-based node value within that extent.
     * 
     * For example, in an extent of 20 units divided into 4 nodes(0, 1, 2, 3),
     * an \a inValue of 8 would be contained within node 1. Values on the
     * border between two nodes are rounded up if possible - an \a inValue of 10 would be contained
     * within node 2, but an \a inValue of 20 would be contained within node 3.
     * 
     * \verbatim
     * 0        5        10       15       20
     * |--------|--------|--------|--------|
     *     0        1         2        3
     * \endverbatim
     * 
     * @note \a extent must be divisible by \a nodeSize. \a inValue should be <= \a extent and >= 0.
     * @param extent The magnitude of the area being split into \a nodeCount areas.
     * @param nodeCount The number of nodes to split \a extent into.
     * @param inValue Single-dimension co-ordinate to remap to a node value, between 0 and \a extent.
     * @return 0-based index of node that this \a inValue resides in, between a minimum of 0
     * and maximum of \a nodeCount - 1.
     */
    static int mapToNodeIndex(int extent, int nodeCount, float inValue);
        
    /**
     * @brief Given the octree's current size, and the magnitude of the distance from the centre of the octree volume
     * to its outer edge, returns the node index specified by the distance along this magnitude.
     * 
     * \note \a magnitude specifies the length of the distance from the origin of the total octree volume to the outermost edge, \b not to the centre
     * of the outermost node. This means that there should be size()/2 nodes between 0 and \a magnitude. If a negative
     * \a inValue is provided, it references nodes in the opposite direction. An \a inValue of 0 will return node size()/2.
     * @param magnitude Number of units from the centre of the octree to its outermost edge.
     * @param inValue Ordinate that lies within \a -magnitude and \a magnitude.
     * @return Node index referenced by \a inValue.
     */
    int mapToNodeIndex(int magnitude, float inValue) const;
        
    /**
     * @brief Returns, given the maximum extent represented by the total octree volume and a given set of co-ordinates
     * within this extent, the contents of the corresponding octree node.
     * 
     * @note All components of \a worldPos should be between \a -magnitude and \a magnitude.
     * @param magnitude Number of units spanned by the distance from the centre of the octree volume to its outermost edge on an axis.
     * @param worldPos A position within the (2 * \a magnitude)^3 volume occupied by the octree.
     * @return The contents of the octree node that encloses this position.
     */
    const T& atPosition(int magnitude, const QVector3D &worldPos) const;

protected:

    // Octree node types
    class Node;
    class Branch;
    class Aggregate;
    class Leaf;
    enum NodeType { BranchNode, AggregateNode, LeafNode };

    /**
     * \brief Gets the octree's root.
     * \return Pointer to octree's root node.
     */
    Node*& root();
    
    /**
     * \brief Const version of root().
     */
    const Node* root() const;

    /**
     * \brief Deletes a node polymorphically. If the node is a branch node, it will delete
     * all its subtree recursively.
     */
    static void deleteNode( Node** node );

private:
    // Recursive helper functions
    /**
     * \brief Helper function for erase() method.
     */
    void eraseRecursive( Node** node, int size, int x, int y, int z );
    
    /**
     * \brief Helper function for bytes() method.
     */
    static unsigned long bytesRecursive( const Node* node );
    
    /**
     * \brief Helper function for nodes() method.
     */
    static int nodesRecursive( const Node* node );
    
    /**
     * \brief Helper function for nodesAtSize() method.
     */
    static int nodesAtSizeRecursive( int targetSize, int size, Node* node );
    
    /**
     * \brief Helper function for zSlice() method.
     */
    void zSliceRecursive( Array2D<T> slice, const Node* node, int size,
            int x, int y, int z, int targetZ ) const;
    
    /**
     * \brief Helper function for writeBinaryRecursive() method.
     */
    static void writeBinaryRecursive( std::ostream& out, const Node* node );
    
    /**
     * \brief Helper function for readBinaryRecursive() method.
     */
    static void readBinaryRecursive( std::istream& in, Node** node );

protected:
    // Node classes

    class Node
    {
    public:
        NodeType type() const;

    protected:
        Node( NodeType type );
        ~Node() {}

    private:
        NodeType type_ : 2;
    };

    class Branch : public Node
    {
    public:
        Branch();
        Branch( const Branch& b );
        ~Branch();

        const Node* child( int x, int y, int z ) const;
        Node*& child( int x, int y, int z );
        const Node* child( int index ) const;
        Node*& child( int index );

        friend void Octree<T,AS>::deleteNode( Node** node );

    private:
        Branch& operator= ( Branch b );

    private:
        Node* children[2][2][2];
    };

    class Aggregate : public Node
    {
    public:
        Aggregate( const T& v );

        const T& value( int x, int y, int z ) const;
        T& value( int x, int y, int z );
        void setValue( int x, int y, int z, const T& v );

        const T& value( int i ) const;
        T& value( int i );
        void setValue( int i, const T& v );

        friend void Octree<T,AS>::deleteNode( Node** node );

    private:
        ~Aggregate() {}

    private:
        T value_[AS][AS][AS];
    };

    class Leaf : public Node
    {
    public:
        Leaf( const T& v );

        const T& value() const;
        T& value();
        void setValue( const T& v );

        friend void Octree<T,AS>::deleteNode( Node** node );

    private:
        ~Leaf() {}

    private:
        T value_;
    };

    static const int aggregateSize_ = AS;

private:
    Node* root_;
    T emptyValue_;
    int size_;
};
    
OCTREE_END_NAMESPACE

// This is included at the end, since templated code doesn't get compiled into a library.
#include "octree.tcc"

#endif
