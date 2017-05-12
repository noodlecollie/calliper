#ifndef PUBLICSTOREDEFS_H
#define PUBLICSTOREDEFS_H

#include "rendersystem_global.h"

#include <QObject>

namespace RenderSystem
{
    class RENDERSYSTEMSHARED_EXPORT PublicStoreDefs
    {
        Q_GADGET
        PublicStoreDefs() = delete;

    public:
        typedef quint32 MaterialId;
        typedef quint32 TextureId;

        static const MaterialId INVALID_MATERIAL_ID;
        static const TextureId INVALID_TEXTURE_ID;
    };
}

#endif // PUBLICSTOREDEFS_H
