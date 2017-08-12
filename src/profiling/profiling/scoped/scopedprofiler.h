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
            const char* const m_szDescription;
            const char* const m_szFunction;
            const char* const m_szFile;

            inline explicit StaticData(const char* description, const char* function, const char* file)
                : m_pModel(Q_NULLPTR),
                  m_pData(Q_NULLPTR),
                  m_szDescription(description),
                  m_szFunction(function),
                  m_szFile(file)
            {
            }

            inline void initialise(ProfilerModel& model)
            {
                m_pModel = &model;
                m_pData = &m_pModel->nextAvailableProfilerData();
            }
        };

        inline ScopedProfiler(StaticData& staticData, ProfilerModel& model, const int line)
            : m_StaticData(staticData),
              m_nMsecOnCreation(QTime::currentTime().msecsSinceStartOfDay())
        {
            if ( !m_StaticData.m_pModel )
            {
                m_StaticData.initialise(model);
            }

            m_StaticData.m_pData->m_nDepth = m_StaticData.m_pModel->onScopedProfilerCreated();
            m_StaticData.m_pData->m_nLine = line;
        }

        inline ~ScopedProfiler()
        {
            m_StaticData.m_pData->m_nTimeInMsec = QTime::currentTime().msecsSinceStartOfDay() - m_nMsecOnCreation;
            m_StaticData.m_pModel->onScopedProfilerDestroyed();
        }

    private:
        StaticData& m_StaticData;
        const int m_nMsecOnCreation;
    };
}

#define SCOPEDPROFILER_STATICDATA_ARGS(_desc) (_desc), Q_FUNC_INFO, __FILE__
#define SCOPEDPROFILER_ARGS(_item, _model) (_item), (_model), __LINE__

#define SCOPED_PROFILER(_desc, _model) \
    static Profiling::ScopedProfiler::StaticData scopedProfilerStaticData(SCOPEDPROFILER_STATICDATA_ARGS(_desc)); \
    Profiling::ScopedProfiler scopedProfiler(SCOPEDPROFILER_ARGS(scopedProfilerStaticData, _model)); \
    Q_UNUSED(scopedProfiler);

#endif // SCOPEDPROFILER_H
