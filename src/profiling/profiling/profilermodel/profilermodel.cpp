#include "profilermodel.h"

namespace Profiling
{
    ProfilerModel::ProfilerModel(quint32 maxDataSlots)
        : m_nMaxDataSlots(maxDataSlots),
          m_pDataArray(new ProfilerData[m_nMaxDataSlots]),
          m_pParentArray(new int[m_nMaxDataSlots]),
          m_nDataSlotCount(0),
          m_nCurrentDepth(0),
          m_nCurrentSlot(NULL_SLOT_ID)
    {
        for ( quint32 i = 0; i < m_nMaxDataSlots; ++i )
        {
            m_pParentArray[i] = INVALID_PARENT_ID;
        }
    }

    ProfilerModel::~ProfilerModel()
    {
        Q_ASSERT_X(m_nCurrentDepth == 0, Q_FUNC_INFO, "ProfilerModel being deleted when scoped profilers are still alive!");

        delete[] m_pParentArray;
        m_pParentArray = Q_NULLPTR;

        delete[] m_pDataArray;
        m_pDataArray = Q_NULLPTR;
    }

    int ProfilerModel::allocateNextAvailableDataSlot()
    {
        if ( m_nDataSlotCount >= m_nMaxDataSlots )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Profiler count exceeded max number of data slots!");
            return NULL_SLOT_ID;
        }

        return m_nDataSlotCount++;
    }

    quint32 ProfilerModel::dataSlotCount() const
    {
        return m_nDataSlotCount;
    }

    QVector<int> ProfilerModel::children(int parentSlot) const
    {
        // There shouldn't be too many profiler slots and we should get cache benefit,
        // so a linear parent search should be OK.

        QVector vec;

        for ( quint32 i = 0; i < m_nDataSlotCount; ++i )
        {
            if ( m_pParentArray[i] == parentSlot )
            {
                vec.append(i);
            }
        }

        return vec;
    }
}
