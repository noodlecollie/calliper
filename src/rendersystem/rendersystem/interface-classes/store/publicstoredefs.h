#ifndef PUBLICSTOREDEFS_H
#define PUBLICSTOREDEFS_H

#include "rendersystem_global.h"

#include <QObject>

namespace RenderSystem
{
    namespace PublicStoreDefs
    {
        typedef quint32 MaterialId;
        typedef quint32 TextureId;

        extern const MaterialId INVALID_MATERIAL_ID;
        extern const TextureId INVALID_TEXTURE_ID;
    }
}

#endif // PUBLICSTOREDEFS_H
