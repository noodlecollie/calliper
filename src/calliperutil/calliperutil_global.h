#ifndef CALLIPERUTIL_GLOBAL_H
#define CALLIPERUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTIL_LIBRARY)
#  define CALLIPERUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CALLIPERUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#define NS_CALLIPERUTIL ModuleCalliperUtil
#define NS_STR_CALLIPERUTIL "ModuleCalliperUtil"

#endif // CALLIPERUTIL_GLOBAL_H
