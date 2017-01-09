#ifndef MODEL_GLOBAL_H
#define MODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MODEL_LIBRARY)
#  define MODELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODELSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Model
{
    extern const char* STR_NAMESPACE;
}

#endif // MODEL_GLOBAL_H
