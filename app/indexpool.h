/** \file indexpool.h
 * \brief TODO
 */

#ifndef INDEXPOOL_H
#define INDEXPOOL_H

#include <QObject>
#include <QMap>
#include "vertex.h"

class IndexPool : public QObject
{
    Q_OBJECT
public:
    explicit IndexPool(QObject *parent = 0, GEOMHANDLE highestIndex = INDEX_LIMIT);
    
    enum
    {
        // 2^32 - 2,
        // Because maxIndex needs to take the value 2^32 - 1 without overflowing.
        INDEX_LIMIT = 4294967294
    };
    
    GEOMHANDLE nextTop() const { return m_iMaxIndex; }
    GEOMHANDLE highestAllocated() const { return m_iMaxIndex - 1; }
    bool isAllocated(GEOMHANDLE index) const;
    bool getHighestAllowedIndex() const { return m_iHighestAllowedIndex; }
    
    // Allocates the lowest free index.
    GEOMHANDLE allocateIndex();
    
signals:
    void indexReallocation(GEOMHANDLE oldValue, GEOMHANDLE newValue);
    
public slots:
    // Releases an index.
    void releaseIndex(const GEOMHANDLE index);
    
    // Condenses all indices - fires indexReallocation for each index which is re-allocated.
    void condense();
private:
    typedef QMap<GEOMHANDLE,GEOMHANDLE> GeoMap;
    
    GEOMHANDLE  m_iMaxIndex;
    GeoMap      m_UnusedIndices;
    GEOMHANDLE  m_iHighestAllowedIndex;
};

#endif // INDEXPOOL_H
