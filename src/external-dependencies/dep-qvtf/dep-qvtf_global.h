#ifndef DEPQVTF_GLOBAL_H
#define DEPQVTF_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DEPQVTF_LIBRARY)
#  define DEPQVTFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DEPQVTFSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Dep_QVtf
{
    static const char* STR_NAMESPACE = "Dep_QVtf";
}

#endif // DEPQVTF_GLOBAL_H
