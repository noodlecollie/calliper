#ifndef FRAMEBUFFERDEFS_H
#define FRAMEBUFFERDEFS_H

#include "rendersystem_global.h"

#include <QtGlobal>

namespace RenderSystem
{
    namespace FrameBufferDefs
    {
        typedef quint16 FrameBufferId;

        RENDERSYSTEMSHARED_EXPORT extern const FrameBufferId INVALID_FRAME_BUFFER_ID;
    }
}

#endif // FRAMEBUFFERDEFS_H
