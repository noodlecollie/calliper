#include "fixedindexpool.h"

#if 0
namespace CalliperUtil
{
    FixedIndexPool::FixedIndexPool(int size)
        : m_iSize(size),
          m_IndexList(new bool[m_iSize]),
          m_iOccupiedIndices(0)
    {

    }

    int FixedIndexPool::size() const
    {
        return m_iSize;
    }

    int FixedIndexPool::occupiedIndices() const
    {
        return m_iOccupiedIndices;
    }

    int FixedIndexPool::getNextIndex()
    {

    }

    int FixedIndexPool::peekNextIndex() const
    {
        for ( int i = 0; i < m_iSize; i++ )
        {
            if ( !m_IndexList[i] )
            {
                return i;
            }
        }

        return -1;
    }

    bool FixedIndexPool::hasNextIndex() const
    {
        return peekNextIndex() != -1;
    }

    int FixedIndexPool::getNextIndex()
    {
        int nextIndex = peekNextIndex();

        if ( nextIndex < 0 )
            return nextIndex;

        m_IndexList[nextIndex] = true;
        m_iOccupiedIndices++;
        Q_ASSERT(m_iOccupiedIndices >= 0 && m_iOccupiedIndices < m_iSize);

        return nextIndex;
    }

    void FixedIndexPool::revokeIndex(int index)
    {
        if ( index < 0 || index >= m_iSize )
            return;

        if ( isIndexOccupied(index) )
            m_iOccupiedIndices--;
        Q_ASSERT(m_iOccupiedIndices >= 0 && m_iOccupiedIndices < m_iSize);

        m_IndexList[index] = false;
    }

    bool FixedIndexPool::isIndexOccupied(int index) const
    {
        return m_IndexList[index];
    }
}
#endif
