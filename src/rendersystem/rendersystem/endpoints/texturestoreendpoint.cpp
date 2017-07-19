#include "texturestoreendpoint.h"
#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"

namespace RenderSystem
{
    namespace TextureStoreEndpoint
    {
        TextureStoreAccessor textureStore()
        {
            return TextureStoreAccessor(*OpenGLTextureStore::globalInstance());
        }

        const ITextureStore* constTextureStore()
        {
            return OpenGLTextureStore::globalInstance();
        }
    }
}
