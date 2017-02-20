#ifndef RENDERER_GLOBAL_H
#define RENDERER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RENDERER_LIBRARY)
#  define RENDERERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RENDERERSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Renderer
{
    extern const char* STR_NAMESPACE;
}

#endif // RENDERER_GLOBAL_H
