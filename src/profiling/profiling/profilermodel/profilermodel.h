#ifndef PROFILERMODEL_H
#define PROFILERMODEL_H

#include "profiling_global.h"

#include <QtGlobal>

namespace Profiling
{
    class PROFILINGSHARED_EXPORT ProfilerModel
    {
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

    private:
        ProfilerData& nextAvailableProfilerData();

        inline quint32 onScopedProfilerCreated()
        {
            return m_nCurrentDepth++;
        }

        inline void onScopedProfilerDestroyed()
        {
            Q_ASSERT_X(m_nCurrentDepth != 0, Q_FUNC_INFO, "Mismatched scoped profiler create/destroy calls!");

            --m_nCurrentDepth;
        }

        const quint32 m_nMaxDataSlots;

        ProfilerData* m_pDataArray;
        quint32 m_nDataSlotCount;
        quint32 m_nCurrentDepth;
    };
}

#endif // PROFILERMODEL_H
