#ifndef RENDERUTILITIES_H
#define RENDERUTILITIES_H

#include "rendersystem_global.h"

#include "rendersystem/interface-classes/definitions/framebufferdefs.h"

namespace RenderSystem
{
    // These functions operate on the currently active context,
    // and don't make the render system context current.
    namespace RenderUtilities
    {
        RENDERSYSTEMSHARED_EXPORT void copyFrameBufferToCurrentSurface(const FrameBufferDefs::FrameBufferId frameBufferId);
    }
}

#endif // RENDERUTILITIES_H
