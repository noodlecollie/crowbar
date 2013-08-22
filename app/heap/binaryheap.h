/**
  \file BinaryHeap.h
  \author Eduard Igushev visit <www.igushev.com> e-mail: <first name at last name dot com>
  \brief Binary Heap C++ implementation
 
  Warranty and license
  The implementation is provided “as it is” with no warranty.
  Any private and commercial usage is allowed.
  Keeping the link to the source is required.
  Any feedback is welcomed :-)
  
  Edited by [X6] Herbius to generalise for supporting both min and max heaps.
  Some stock code modified.
*/
 
#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H
 
#include <vector>
#include <iterator>
#include <cmath>
 
template <class T, class ISMAX>
class BinaryHeap {
  std::vector<T> _heap;
 
public:
  BinaryHeap() { /* Validate: */ bool b = ISMAX; }
  BinaryHeap(T array[], size_t size);
  ~BinaryHeap() {}
 
  void Insert(const T&);
  T PopTop();
  const T& Top() const { return _heap.front(); }
  unsigned Count() const { return _heap.size(); }
  bool IsMax() const { return ISMAX; }
  bool IsEmpty() const { return _heap.size() < 1; }
  void Clean(bool dealloc = true) { dealloc ? vector<T>().swap(_heap) : _heap.Clear(); }
 
private:
  typedef typename std::vector<T>::size_type _heap_size_t;
 
  void _buildHeap();
  void _shiftDown(_heap_size_t node);
  void _shiftUp(_heap_size_t node);
};
 
template <class T, class ISMAX>
BinaryHeap<T, ISMAX>::BinaryHeap(T array[], size_t size)
{
  // Validate:
  bool b = ISMAX;
  _heap.reserve(size);
  
  for ( size_t i = 0; i < size; i++ )
  {
      _heap.push_back(array[i]);
  }
  
  _buildHeap();
}
 
template <class T, class ISMAX>
void BinaryHeap<T, ISMAX>::Insert(const T& value)
{
  _heap.push_back(value);
  _shiftUp(_heap.size()-1);
}
 
template <class T, class ISMAX>
T BinaryHeap<T, ISMAX>::PopTop()
{
  T result = _heap.front();
  _heap.front() = _heap.back();
  _heap.pop_back();
  _shiftDown(0);
  return result;
}
 
template <class T, class ISMAX>
void BinaryHeap<T, ISMAX>::_buildHeap()
{
  for (_heap_size_t i = _heap.size()/2; i >= 0; --i)
    _shiftDown(i);
}
 
template <class T, class ISMAX>
void BinaryHeap<T, ISMAX>::_shiftDown(_heap_size_t node)
{
  _heap_size_t left_child = node*2+1;   // This node's left child
  _heap_size_t right_child = node*2+2;  // This node's right child
 
  _heap_size_t replace = node;          // Initially set up our node to swap as our current node.
  if (right_child < _heap.size())       // If the node has a valid right child:
  {
    if ( ISMAX == true )    // Prodecure for max-heap:
    {
        bool left = _heap[right_child] < _heap[left_child]; // True if right < left, false if left < right.
        if (left && _heap[node] < _heap[left_child])        // If right child and our node are < left child:
          replace = left_child;                             // We will swap our node with the left child.
        else if (!left && _heap[node] < _heap[right_child]) // If left child and node < right child:
          replace = right_child;                            // We will swap our node with the right child.
    }
    else            // Procedure for min-heap:
    {
        bool left = _heap[right_child] > _heap[left_child]; // True if right > left, false if left > right.
        if (left && _heap[node] > _heap[left_child])        // If right child and our node are > left child:
          replace = left_child;                             // We will swap our node with the left child.
        else if (!left && _heap[node] > _heap[right_child]) // If left child and node > right child:
          replace = right_child;                            // We will swap our node with the right child.
    }
  }
  else if (left_child < _heap.size())                   // If we have no right child but do have a left child:
  {
    if ( ISMAX == true )    // Procedure for max-heap:
    {
        if (_heap[node] < _heap[left_child])                // If the node is < its child:
          replace = left_child;                             // We will swap our node with the left child.
    }
    else            // Procedure for min-heap:
    {
        if (_heap[node] > _heap[left_child])                // If the node is > its child:
          replace = left_child;                             // We will swap our node with the left child.
    }
  }
 
  if (replace == node)                                  // If we didn't need to swap with any children, return.
    return;
  std::swap(_heap[node], _heap[replace]);               // Swap our node with the child.
  _shiftDown(replace);                                  // Sort the child.
}
 
template <class T, class ISMAX>
void BinaryHeap<T, ISMAX>::_shiftUp(_heap_size_t node)
{
  if (node == 0)
    return;
 
  _heap_size_t parent = std::floor((node-1)/2); // Calculate the parent of this node.
 
  if ( ISMAX == true ) // Procedure for max-heap:
  {
    if (_heap[node] < _heap[parent])              // If we're already correctly ordered, return.
        return;
  }
  else          // Procedure for min-heap:
  {
    if (_heap[node] > _heap[parent])              // If we're already correctly ordered, return.
        return;
  }
  std::swap(_heap[node], _heap[parent]);        // Swap the nodes.
  _shiftUp(parent);                             // Sort the parent.
}

template <class T>
class BinaryHeapMin : BinaryHeap<T, false>
{
public:
    BinaryHeapMin() : BinaryHeap()
    {
    }
    
    BinaryHeapMin(T array[], size_t size) : BinaryHeap(array, size)
    {
    }
    
    ~BinaryHeapMin()
    {
    }
    
    // Min-specific functions:
    T PopMin()                  { return this->PopTop(); }
    const T& Min() const        { return this->Top(); }
};

template <class T>
class BinaryHeapMax : BinaryHeap<T, true>
{
public:
    BinaryHeapMax() : BinaryHeap()
    {
    }
    
    BinaryHeapMax(T array[], size_t size) : BinaryHeap(array, size)
    {
    }
    
    ~BinaryHeapMax()
    {
    }
    
    // Max-specific functions:
    T PopMax()                  { return this->PopTop(); }
    const T& Max() const        { return this->Top(); }
};
 
#endif
