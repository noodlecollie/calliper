#include "framebufferoperationsendpoint.h"
#include "rendersystem/private/framebuffer/framebufferoperations.h"

namespace RenderSystem
{
    namespace FrameBufferOperationsEndpoint
    {
        FrameBufferOperationsAccessor frameBufferOperations()
        {
            // As the class currently doesn't store any state,
            // we don't need to control its creation and destruction.
            // Keeping it on the stack should work for now.
            static FrameBufferOperations frameBufferOperationsInstance;

            return FrameBufferOperationsAccessor(frameBufferOperationsInstance);
        }
    }
}
