#ifndef SCOPEDPROFILER_H
#define SCOPEDPROFILER_H

#include <QTime>

#include "profiling/profilermodel/profilermodel.h"

namespace Profiling
{
    class ScopedProfiler
    {
    public:
        struct StaticData
        {
            ProfilerModel* m_pModel;
            ProfilerModel::ProfilerData* m_pData;
            int m_nDataSlot;
            const char* const m_szDescription;
            const char* const m_szFunction;
            const char* const m_szFile;

            inline explicit StaticData(const char* description, const char* function, const char* file)
                : m_pModel(Q_NULLPTR),
                  m_pData(Q_NULLPTR),
                  m_nDataSlot(-1),
                  m_szDescription(description),
                  m_szFunction(function),
                  m_szFile(file)
            {
            }

            inline void ensureInitialised(ProfilerModel& model)
            {
                if ( m_pModel )
                {
                    return;
                }

                m_pModel = &model;
                m_nDataSlot = m_pModel->allocateNextAvailableDataSlot();
                m_pData = &m_pModel->m_pDataArray[m_nDataSlot];
            }
        };

        inline ScopedProfiler(StaticData& staticData, ProfilerModel& model, const int line)
            : m_StaticData(staticData),
              m_nMsecOnCreation(QTime::currentTime().msecsSinceStartOfDay())
        {
            m_StaticData.ensureInitialised(model);

            m_StaticData.m_pData->m_nDepth = m_StaticData.m_pModel->onScopedProfilerCreated(m_StaticData.m_nDataSlot);
            m_StaticData.m_pData->m_nLine = line;
        }

        inline ~ScopedProfiler()
        {
            m_StaticData.m_pData->m_nTimeInMsec = QTime::currentTime().msecsSinceStartOfDay() - m_nMsecOnCreation;
            m_StaticData.m_pModel->onScopedProfilerDestroyed(m_StaticData.m_nDataSlot);
        }

    private:
        StaticData& m_StaticData;
        const int m_nMsecOnCreation;
    };
}

#ifdef ENABLE_PROFILING
#define SCOPEDPROFILER_STATICDATA_ARGS(_desc) (_desc), Q_FUNC_INFO, __FILE__
#define SCOPEDPROFILER_ARGS(_item, _model) (_item), (_model), __LINE__

#define SCOPED_PROFILER(_desc, _model) \
    static Profiling::ScopedProfiler::StaticData scopedProfilerStaticData(SCOPEDPROFILER_STATICDATA_ARGS(_desc)); \
    Profiling::ScopedProfiler scopedProfiler(SCOPEDPROFILER_ARGS(scopedProfilerStaticData, _model)); \
    Q_UNUSED(scopedProfiler);
#else
#define SCOPEDPROFILER_STATICDATA_ARGS(...)
#define SCOPEDPROFILER_ARGS(...)
#define SCOPED_PROFILER(...)
#endif

#endif // SCOPEDPROFILER_H
