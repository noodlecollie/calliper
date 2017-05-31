#ifndef PUBLICTEXTUREDEFS_H
#define PUBLICTEXTUREDEFS_H

#include "rendersystem_global.h"

#include <QObject>

namespace RenderSystem
{
    namespace PublicTextureDefs
    {
        enum TextureUnit
        {
            MainTexture = 0,
            SecondaryTexture = 1,
            NormalMap = 2,
        };
    }
}

#endif // PUBLICTEXTUREDEFS_H
