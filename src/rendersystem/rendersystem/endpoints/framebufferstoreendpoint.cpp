#include "framebufferstoreendpoint.h"
#include "rendersystem/private/stores/framebufferstore/framebufferstore.h"

namespace RenderSystem
{
    namespace FrameBufferStoreEndpoint
    {
        FrameBufferStoreAccessor frameBufferStore()
        {
            return FrameBufferStoreAccessor(*FrameBufferStore::globalInstance());
        }
    }
}
