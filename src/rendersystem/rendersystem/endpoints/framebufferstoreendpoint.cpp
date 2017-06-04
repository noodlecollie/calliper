#include "framebufferstoreendpoint.h"
#include "rendersystem/private/stores/framebufferstore.h/framebufferstore.h"

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
