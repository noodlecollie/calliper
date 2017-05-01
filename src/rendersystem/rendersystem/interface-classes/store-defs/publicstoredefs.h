#ifndef PUBLICSTOREDEFS_H
#define PUBLICSTOREDEFS_H

#include "rendersystem_global.h"

#include <QObject>

namespace RenderSystem
{
    class PublicStoreDefs
    {
        Q_GADGET
        PublicStoreDefs() = delete;
    public:
        typedef quint32 MaterialId;
        typedef quint32 TextureId;
    };
}

#endif // PUBLICSTOREDEFS_H
