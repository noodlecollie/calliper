#ifndef PUBLICTEXTUREDEFS_H
#define PUBLICTEXTUREDEFS_H

#include "rendersystem_global.h"

#include <QObject>

namespace RenderSystem
{
    class PublicTextureDefs
    {
        Q_GADGET
        PublicTextureDefs() = delete;
    public:
        enum TextureUnit
        {
            MainTexture = 0,
            SecondaryTexture = 1,
            NormalMap = 2,
        };
        Q_ENUM(TextureUnit)
    };
}

#endif // PUBLICTEXTUREDEFS_H
