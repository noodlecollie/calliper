#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"
#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"

namespace RenderSystem
{
    const PublicStoreDefs::MaterialId PublicStoreDefs::INVALID_MATERIAL_ID = MaterialStore::INVALID_ID;
    const PublicStoreDefs::TextureId PublicStoreDefs::INVALID_TEXTURE_ID = OpenGLTextureStore::INVALID_ID;
}
