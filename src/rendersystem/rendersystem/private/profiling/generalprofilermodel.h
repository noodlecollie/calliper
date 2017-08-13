#ifndef GENERALPROFILERMODEL_H
#define GENERALPROFILERMODEL_H

#include "profiling/profilermodel/profilermodel.h"
#include "calliperutil/global/globalinstancehelper.h"

class GeneralProfilerModel : public Profiling::ProfilerModel,
                             public CalliperUtil::GlobalInstanceHelper<GeneralProfilerModel>
{
public:
    GeneralProfilerModel()
        : Profiling::ProfilerModel(64)
    {
    }
};

#endif // GENERALPROFILERMODEL_H
