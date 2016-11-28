#ifndef MODELLOADERS_GLOBAL_H
#define MODELLOADERS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MODELLOADERS_LIBRARY)
#  define MODELLOADERSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODELLOADERSSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace ModelLoaders
{
    static const char* STR_NAMESPACE = "MapImporters";
}

#endif // MODELLOADERS_GLOBAL_H
