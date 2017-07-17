#ifndef TEXTUREDEFS_H
#define TEXTUREDEFS_H

#include "rendersystem_global.h"

namespace RenderSystem
{
    namespace TextureDefs
    {
        typedef quint32 TextureId;

        RENDERSYSTEMSHARED_EXPORT extern const TextureId INVALID_TEXTURE_ID;

        enum TextureUnit
        {
            MainTexture = 0,
            SecondaryTexture = 1,
            NormalMap = 2,

            TOTAL_TEXTURE_UNITS
        };

        enum RenderTarget
        {
            NoRenderTarget = -1,

            RenderTarget0 = 0,

            TOTAL_RENDER_TARGETS
        };
    }
}

#endif // TEXTUREDEFS_H
