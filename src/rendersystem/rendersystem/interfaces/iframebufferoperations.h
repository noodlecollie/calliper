#ifndef IFRAMEBUFFEROPERATIONS_H
#define IFRAMEBUFFEROPERATIONS_H

#include <QOpenGLFramebufferObject>

#include "rendersystem/interface-classes/definitions/framebufferdefs.h"

namespace RenderSystem
{
    class IFrameBufferOperations
    {
    public:
        virtual ~IFrameBufferOperations() {}

        // Saves the contents of the frame buffer to the specified file.
        virtual void saveToFile(const FrameBufferDefs::FrameBufferId id, const QString& filename) = 0;
    };
}

#endif // IFRAMEBUFFEROPERATIONS_H
