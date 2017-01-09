#ifndef DEPVTFLIB_GLOBAL_H
#define DEPVTFLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DEPVTFLIB_LIBRARY)
#  define DEPVTFLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DEPVTFLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Dep_VTFLib
{
    extern const char* STR_NAMESPACE;
}

#endif // DEPVTFLIB_GLOBAL_H
