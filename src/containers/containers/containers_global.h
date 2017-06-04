#ifndef CONTAINERS_GLOBAL_H
#define CONTAINERS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CONTAINERS_LIBRARY)
#  define CONTAINERSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONTAINERSSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Containers
{
    CONTAINERSSHARED_EXPORT extern const char* STR_NAMESPACE;
}

#endif // CONTAINERS_GLOBAL_H
