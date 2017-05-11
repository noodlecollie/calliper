#ifndef PUBLICRENDERMODELDEFS_H
#define PUBLICRENDERMODELDEFS_H

#include "rendersystem_global.h"

#include <QObject>

namespace RenderSystem
{
    class PublicRenderModelDefs
    {
        Q_GADGET
        PublicRenderModelDefs() = delete;
    public:
        typedef quint32 ObjectId;
        typedef quint8 SectionId;
    };
}

#endif // PUBLICRENDERMODELDEFS_H
