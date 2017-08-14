#include "profilermodel.h"

namespace
{
    static constexpr int NULL_SLOT_ID = -1;
}

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

    void ProfilerModel::exportChildren(QVector<QVector<int> > &childArrays) const
    {
        // There are count+1 arrays, because we want to include the children of the null slot.
        childArrays.clear();
        childArrays.reserve(m_nDataSlotCount + 1);

        for ( int i = 0; i <= static_cast<int>(m_nDataSlotCount); ++i )
        {
            childArrays.append(QVector<int>());
        }

        for ( int slot = 0; slot < static_cast<int>(m_nDataSlotCount); ++slot )
        {
            const int parentOfSlot = m_pParentArray[slot];
            if ( parentOfSlot == INVALID_PARENT_ID )
            {
                continue;
            }

            Q_ASSERT_X(parentOfSlot + 1 < childArrays.count(), Q_FUNC_INFO, "Parent slot index out of range!");

            childArrays[parentOfSlot + 1].append(slot);
        }
    }

    int ProfilerModel::parent(int childSlot) const
    {
        if ( childSlot < 0 || childSlot >= static_cast<int>(m_nDataSlotCount) )
        {
            return NULL_SLOT_ID;
        }

        return m_pParentArray[childSlot];
    }
}
