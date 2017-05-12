#ifndef TEXTURESTOREENDPOINT_H
#define TEXTURESTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/itexturestore.h"

namespace RenderSystem
{
    namespace TextureStoreEndpoint
    {
        RENDERSYSTEMSHARED_EXPORT CurrentContextGuard<ITextureStore> textureStore();
    }
}
#endif // TEXTURESTOREENDPOINT_H
