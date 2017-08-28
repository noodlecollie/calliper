#ifndef PROFILERMODEL_H
#define PROFILERMODEL_H

#include "profiling_global.h"

#include <QObject>
#include <QVector>

namespace Profiling
{
    // This class is designed to be fast when scoped profilers are created and destroyed.
    // Other non-live functions bear the brunt of the later work.
    class PROFILINGSHARED_EXPORT ProfilerModel : public QObject
    {
        Q_OBJECT
        friend class ScopedProfiler;
    public:
        struct ProfilerData
        {
            const char* m_szDescription;
            const char* m_szFunction;
            const char* m_szFile;
            int m_nLine;
            quint32 m_nDepth;
            int m_nTimeInMsec;

            inline ProfilerData()
            {
                memset(this, 0, sizeof(ProfilerData));
            }
        };

        ProfilerModel(quint32 maxDataSlots);
        ~ProfilerModel();

        quint32 dataSlotCount() const;
        int parent(int childSlot) const;

        void exportChildren(QVector<QVector<int> >& childArrays) const;

    signals:
        void depthReachedZero();

    private:
        int allocateNextAvailableDataSlot();

        inline quint32 onScopedProfilerCreated(const int slotIndex)
        {
            Q_ASSERT_X(slotIndex >= 0 && slotIndex < static_cast<int>(m_nDataSlotCount), Q_FUNC_INFO, "Slot index out of range!");

            if ( m_pParentArray[slotIndex] == INVALID_PARENT_ID )
            {
                m_pParentArray[slotIndex] = m_nCurrentSlot;
            }

            m_nCurrentSlot = slotIndex;
            return m_nCurrentDepth++;
        }

        inline void onScopedProfilerDestroyed(const int slotIndex)
        {
            Q_ASSERT_X(m_nCurrentDepth > 0, Q_FUNC_INFO, "Mismatched scoped profiler create/destroy calls!");
            Q_ASSERT_X(slotIndex == m_nCurrentSlot, Q_FUNC_INFO, "Non-current slot being destroyed!");

            --m_nCurrentDepth;

            if ( m_nCurrentDepth == 0 )
            {
                emit depthReachedZero();
            }

            m_nCurrentSlot = m_pParentArray[m_nCurrentSlot];
            Q_ASSERT_X(m_nCurrentSlot != INVALID_PARENT_ID, Q_FUNC_INFO, "Invalid parent ID encountered!");
        }

        static constexpr int INVALID_PARENT_ID = -2;

        const quint32 m_nMaxDataSlots;

        ProfilerData* m_pDataArray;
        int* m_pParentArray;
        quint32 m_nDataSlotCount;
        quint32 m_nCurrentDepth;
        int m_nCurrentSlot;
    };
}

#endif // PROFILERMODEL_H
