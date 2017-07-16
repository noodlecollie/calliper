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

        // Renders the contents of the frame buffer into the current buffer (eg. a QOpenGLWidget being painted).
        // The provided frame buffer is used as a texture to draw a full-screen quad.
        // I have no idea what would happen if the current buffer was the same as the frame buffer passed in,
        // so probably best not to do that unless you're foolhardy.
        virtual void drawContentsToCurrent(const FrameBufferDefs::FrameBufferId id) = 0;
    };
}

#endif // IFRAMEBUFFEROPERATIONS_H
