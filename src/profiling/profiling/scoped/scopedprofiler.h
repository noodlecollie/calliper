#ifndef SCOPEDPROFILER_H
#define SCOPEDPROFILER_H

#include <QString>
#include <QTime>

#include "profiling/profilermodel/profilermodel.h"

namespace Profiling
{
    class ScopedProfiler
    {
    public:
        inline ScopedProfiler(ProfilerModel& model, const QString& description, const QString& function, const QString& file, int line)
            : m_strDescription(description),
              m_strFunction(function),
              m_strFile(file),
              m_nLine(line),
              m_nMsecOnCreation(QTime::currentTime().msecsSinceStartOfDay())
        {
        }

        inline ~ScopedProfiler()
        {
        }

        inline const QString& description() const
        {
            return m_strDescription;
        }

        inline const QString& function() const
        {
            return m_strFunction;
        }

        inline const QString& file() const
        {
            return m_strFile;
        }

        inline int line() const
        {
            return m_nLine;
        }

        inline int msecOnCreation() const
        {
            return m_nMsecOnCreation;
        }

    private:
        ProfilerModel m_Model;
        const QString m_strDescription;
        const QString m_strFunction;
        const QString m_strFile;
        const int m_nLine;
        const int m_nMsecOnCreation;
    };
}

#endif // SCOPEDPROFILER_H
