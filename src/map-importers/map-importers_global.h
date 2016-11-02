#ifndef MAPIMPORTERS_GLOBAL_H
#define MAPIMPORTERS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAPIMPORTERS_LIBRARY)
#  define MAPIMPORTERSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAPIMPORTERSSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace MapImporters
{
    static const char* STR_NAMESPACE = "MapImporters";
}

#endif // MAPIMPORTERS_GLOBAL_H
