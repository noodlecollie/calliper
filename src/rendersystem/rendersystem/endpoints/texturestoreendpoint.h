#ifndef TEXTURESTOREENDPOINT_H
#define TEXTURESTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/itexturestore.h"

namespace RenderSystem
{
    namespace TextureStoreEndpoint
    {
        typedef CurrentContextGuard<ITextureStore> TextureStoreAccessor;

        RENDERSYSTEMSHARED_EXPORT TextureStoreAccessor textureStore();
        RENDERSYSTEMSHARED_EXPORT const ITextureStore* constTextureStore();
    }
}

#endif // TEXTURESTOREENDPOINT_H
