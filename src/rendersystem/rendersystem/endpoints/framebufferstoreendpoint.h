#ifndef FRAMEBUFFERSTOREENDPOINT_H
#define FRAMEBUFFERSTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/iframebufferstore.h"

namespace RenderSystem
{
    namespace FrameBufferStoreEndpoint
    {
        typedef CurrentContextGuard<IFrameBufferStore> FrameBufferStoreAccessor;

        RENDERSYSTEMSHARED_EXPORT FrameBufferStoreAccessor frameBufferStore();
    }
}

#endif // FRAMEBUFFERSTOREENDPOINT_H
