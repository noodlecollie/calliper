#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "rendersystem_global.h"

namespace RenderSystem
{
    namespace Global
    {
        RENDERSYSTEMSHARED_EXPORT bool initialise();
        RENDERSYSTEMSHARED_EXPORT void shutdown();

        RENDERSYSTEMSHARED_EXPORT bool makeCurrent();
        RENDERSYSTEMSHARED_EXPORT void doneCurrent();
    }
}

#endif // RENDERSYSTEM_H
