#include "texturestoreendpoint.h"
#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"

namespace RenderSystem
{
    namespace TextureStoreEndpoint
    {
        CurrentContextGuard<ITextureStore> textureStore()
        {
            return CurrentContextGuard<ITextureStore>(*OpenGLTextureStore::globalInstance());
        }
    }
}
