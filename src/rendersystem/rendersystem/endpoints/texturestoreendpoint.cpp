#include "texturestoreendpoint.h"
#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"

namespace
{
    OpenGLTextureStore localTextureStore;
}

namespace RenderSystem
{
    CurrentContextGuard<ITextureStore> textureStoreEndpoint()
    {
        return CurrentContextGuard<ITextureStore>(localTextureStore);
    }
}
