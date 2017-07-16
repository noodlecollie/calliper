#ifndef FRAMEBUFFEROPERATIONSENDPOINT_H
#define FRAMEBUFFEROPERATIONSENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/iframebufferoperations.h"

namespace RenderSystem
{
    namespace FrameBufferOperationsEndpoint
    {
        typedef CurrentContextGuard<IFrameBufferOperations> FrameBufferOperationsAccessor;

        RENDERSYSTEMSHARED_EXPORT FrameBufferOperationsAccessor frameBufferOperations();
    }
}

#endif // FRAMEBUFFEROPERATIONSENDPOINT_H
