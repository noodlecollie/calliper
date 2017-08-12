#ifndef PROFILING_GLOBAL_H
#define PROFILING_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROFILING_LIBRARY)
#  define PROFILINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PROFILINGSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Profiling
{
    PROFILINGSHARED_EXPORT extern const char* STR_NAMESPACE;
}

#endif // PROFILING_GLOBAL_H
