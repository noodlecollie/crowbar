#include "indexpool.h"

IndexPool::IndexPool(QObject *parent, GEOMHANDLE highestIndex) :
    QObject(parent), m_iMaxIndex(1), m_UnusedIndices()
{
    Q_ASSERT(highestIndex > 0 && highestIndex <= INDEX_LIMIT);
    m_iHighestAllowedIndex = highestIndex;
}

GEOMHANDLE IndexPool::allocateIndex()
{
    // Don't go over the max index amount.
    if ( m_iMaxIndex >= m_iHighestAllowedIndex || m_iMaxIndex >= INDEX_LIMIT ) return NULLHND;
    
    // If unused indices is empty, the next index on is m_iMaxIndex.
    if ( m_UnusedIndices.isEmpty() )
    {
        Q_ASSERT(m_iMaxIndex > 0);
        GEOMHANDLE temp = m_iMaxIndex;
        m_iMaxIndex++;
        return temp;
    }
    
    // If not, remove and return the lowest unused index.
    // (Assuming lowest index is stored at lowest map position).
    GeoMap::iterator i = m_UnusedIndices.begin();
    Q_ASSERT(i.value() > 0);
    GEOMHANDLE temp = i.value();
    m_UnusedIndices.erase(i);
    return temp;
}

void IndexPool::releaseIndex(const GEOMHANDLE index)
{
    if ( index > highestAllocated() || index <= 0 ) return;

    // Index is between 1 and highest allocated (inclusive).
    // If it is currently in the unused list, this function
    // will only overwrite it since both the key and the value
    // are identical, hence no need to search first.
    m_UnusedIndices.insert(index, index);
    
    // If we just removed the highest allocated index, there's no guarantee that the next lowest
    // index is being used. We could have a queue of unused indices before we reach the next highest
    // used index. We need to cycle back through the unused indices map, removing each unused index
    // until we come across a used one. If the gap between the index we are checking and the previous
    // index in the map is greater than one, we know there is a used index directly after the last
    // checked, so we should set the max allocated index to this index.
    
    // End element is invalid, so move back one. We know there will be at least one element in the map.
    GeoMap::iterator i = m_UnusedIndices.end()--;
    
    // If i is less than the top allocated, top is in use so we can quit.
    if ( i.value() < highestAllocated() ) return;
    
    // Set up variable to hold last index we found.
    GEOMHANDLE lastIndex = highestAllocated() + 1;
    
    do
    {
        // If there is more than a difference of 1 between the last index and this handle,
        // lastIndex-1 is the new highest allocated index. m_iMaxIndex is set to
        // highestAllocated + 1, so we just set it to lastIndex.
        if ( (lastIndex - i.value()) > 1 )
        {
            m_iMaxIndex = lastIndex;
            Q_ASSERT(m_iMaxIndex > 0);
            return;
        }
        
        // The last checked index and the current are consecutive.
        // Update the last checked and shift back one element, erasing the old one.
        lastIndex = i.value();
        GeoMap::iterator j = i;
        i--;
        
        m_UnusedIndices.erase(j);   // Erase the old element.
    }
    // If we are not left on the beginning element, loop again.
    while ( i != m_UnusedIndices.begin() );
    
    // We moved back until we reached the beginning element in the map, which means one of two things:
    // 1. The rest of the in-use indices are contiguous, ending at i.value() - 1 with the end index being > 0.
    // 2. There are no more indices in use - i.value() is 1.
    // Both mean the map needs to be cleared out completely and maxIndex needs to be set to i.value().
    
    m_iMaxIndex = i.value();
    Q_ASSERT(m_iMaxIndex > 0);
    m_UnusedIndices.clear();
    return;
}

bool IndexPool::isAllocated(GEOMHANDLE index) const
{
    // An index is allocated if:
    // 0 < index < m_iMaxIndex.
    // It is not present in m_UnusedIndices.
    
    return ( index > NULLHND && index < m_iMaxIndex && m_UnusedIndices.find(index) == m_UnusedIndices.end() );
}

void IndexPool::condense()
{
    // If we have no unused indices, we don't need to condense!
    if ( m_UnusedIndices.isEmpty() ) return;
    
    // If the map is not empty, we are guaranteed to have at least one index that is in use.
    // This may be the max index.
    GeoMap::iterator i = m_UnusedIndices.begin();
    GEOMHANDLE lastAssigned = i.value() - 1;
    GEOMHANDLE lastChecked = lastAssigned;
    Q_ASSERT(i.value() > 0);
    
    // While the element is valid:
    while ( i != m_UnusedIndices.end() )
    {
        // If the difference between the last index and this one is > 1, we have skipped over some used indices.
        if ( (i.value() - lastChecked) > 1 )
        {
            // For each of the indices in-between, notify a change.
            for ( GEOMHANDLE n = lastChecked+1; n < i.value(); n++ )
            {
                // The "change" is firing a signal containing the old value and the new value.
                // External systems should hook into this and update their records.
                emit indexReallocation(n, lastAssigned + 1);
                lastAssigned++;
            }
        }
        
        // Mark the current element as the last one checked.
        lastChecked = i.value();
        
        // Erase the current element and move on.
        i = m_UnusedIndices.erase(i);
    }
    
    // We haven't finished yet! There is still a contiguous block of used indices from lastChecked + 1 to highestAllocated inclusive.
    // Go through each of these and re-allocate them.
    
    for ( GEOMHANDLE n = lastChecked+1; n <= highestAllocated(); n++ )
    {
        emit indexReallocation(n, lastAssigned + 1);
        lastAssigned++;
    }
    
    // Reset m_iMaxIndex to lastAssigned + 1.
    m_iMaxIndex = lastAssigned + 1;
}
