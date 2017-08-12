#include "profilermodel.h"

namespace Profiling
{
    ProfilerModel::ProfilerModel(quint32 maxDataSlots)
        : m_nMaxDataSlots(maxDataSlots),
          m_pDataArray(new ProfilerData[m_nMaxDataSlots]),
          m_nDataSlotCount(0),
          m_nCurrentDepth(0)
    {

    }

    ProfilerModel::~ProfilerModel()
    {
        Q_ASSERT_X(m_nCurrentDepth == 0, Q_FUNC_INFO, "ProfilerModel being deleted when scoped profilers are still alive!");

        delete[] m_pDataArray;
        m_pDataArray = Q_NULLPTR;
    }

    ProfilerModel::ProfilerData& ProfilerModel::nextAvailableProfilerData()
    {
        static ProfilerData DummyData;

        if ( m_nDataSlotCount >= m_nMaxDataSlots )
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Profiler count exceeded max number of data slots!");
            return DummyData;
        }

        return m_pDataArray[m_nDataSlotCount++];
    }
}
