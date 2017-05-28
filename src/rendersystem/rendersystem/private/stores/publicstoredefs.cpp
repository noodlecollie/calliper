#include "rendersystem/interface-classes/store/publicstoredefs.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"
#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"

namespace RenderSystem
{
    namespace PublicStoreDefs
    {
        const MaterialId INVALID_MATERIAL_ID = MaterialStore::INVALID_ID;
        const TextureId INVALID_TEXTURE_ID = OpenGLTextureStore::INVALID_ID;
    }
}
