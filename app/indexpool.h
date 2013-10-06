/** \file indexpool.h
 * \brief Defines the IndexPool class which manages non-consecutive array indices.
 */

#ifndef INDEXPOOL_H
#define INDEXPOOL_H

#include <QObject>
#include <QMap>
#include "vertex.h"

/**
 * @brief The IndexPool class manages non-consecutive array indices.
 *
 * The index pool works on the following heuristics:<br/>
 * <ul><li>If the current number of allocated indices is contiguous (or nothing), the new index is
 * allocated at the end of the current block of indices.</li>
 * <li>If the current number of allocated indices is non-contiguous, the lowest unused index is allocated.</li></ul><br/>
 * This means that when indices are deallocated, "holes" in the array of indices are filled first before
 * new indices are added to the end of the array.
 */
class IndexPool : public QObject
{
    Q_OBJECT
public:
    
    /**
     * @brief Constructor.
     * @param parent Parent object.
     * @param highestIndex Highest number of indices.
     */
    explicit IndexPool(QObject *parent = 0, GEOMHANDLE highestIndex = INDEX_LIMIT);
    
    virtual ~IndexPool() {}
    
    enum
    {
        // 2^32 - 2,
        // Because maxIndex needs to take the value 2^32 - 1 without overflowing.
        INDEX_LIMIT = 4294967294    /**< Highest allocatable index. */
    };
    
    /**
     * @brief Returns whether the specified index is currently allocated.
     * @param index Index to check.
     * @return True if already allocated, false if not.
     */
    bool isAllocated(GEOMHANDLE index) const;
    
    /**
     * @brief Returns the maximum index which can be allocated by this pool.
     * @return Highest allowed index.
     */
    bool getHighestAllowedIndex() const { return m_iHighestAllowedIndex; }
    
    /**
     * @brief Allocates the lowest free index.
     * @return Allocated index, or NULLHND if no more indices could be allocated.
     */
    GEOMHANDLE allocateIndex();
    
signals:
    /**
     * @brief Fired when indices are re-allocated by condense(). Any indices whose value does not
     * get passed through this signal as oldValue remains at its original value.
     * @param oldValue Old value of index.
     * @param newValue The index's new value.
     */
    void indexReallocation(GEOMHANDLE oldValue, GEOMHANDLE newValue);
    
public slots:
    /**
     * @brief Releases an index, allowing it to be allocated again.
     * @param index Index to release.
     */
    void releaseIndex(const GEOMHANDLE index);
    
    /**
     * @brief Condenses all indices (shifts indices down to fill all "holes" in the array).
     * indexReallocation is fired for each re-allocated index.
     */
    void condense();
private:
    typedef QMap<GEOMHANDLE,GEOMHANDLE> GeoMap;
    
    /**
     * @brief Returns the next index which is higher than the current max index.
     * @return Current max index + 1.
     */
    GEOMHANDLE nextTop() const { return m_iMaxIndex; }
    
    /**
     * @brief Returns the highest currently allocated index.
     * @return Current max index.
     */
    GEOMHANDLE highestAllocated() const { return m_iMaxIndex - 1; }
    
    GEOMHANDLE  m_iMaxIndex;            /**< Points to next highest free index in the list. */
    GeoMap      m_UnusedIndices;        /**< Map to hold indices of "holes" in the index list. */
    GEOMHANDLE  m_iHighestAllowedIndex; /**< Indices above this value cannot be allocated. */
};

#endif // INDEXPOOL_H
