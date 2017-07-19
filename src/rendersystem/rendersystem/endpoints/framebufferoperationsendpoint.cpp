#include "framebufferoperationsendpoint.h"
#include "rendersystem/private/framebuffer/framebufferoperations.h"

namespace
{
    // As the class currently doesn't store any state,
    // we don't need to control its creation and destruction.
    // Keeping it on the stack should work for now.
    FrameBufferOperations frameBufferOperationsInstance;
}

namespace RenderSystem
{
    namespace FrameBufferOperationsEndpoint
    {
        FrameBufferOperationsAccessor frameBufferOperations()
        {
            return FrameBufferOperationsAccessor(frameBufferOperationsInstance);
        }

        const IFrameBufferOperations* constFrameBufferOperations()
        {
            return &frameBufferOperationsInstance;
        }
    }
}
