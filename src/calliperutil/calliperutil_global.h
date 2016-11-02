#ifndef CALLIPERUTIL_GLOBAL_H
#define CALLIPERUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTIL_LIBRARY)
#  define CALLIPERUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CALLIPERUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace CalliperUtil
{
    static const char* STR_NAMESPACE = "CalliperUtil";
}

#endif // CALLIPERUTIL_GLOBAL_H
