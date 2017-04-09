#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include "renderer_global.h"
#include <QOpenGLFramebufferObject>

namespace Renderer
{
    // TODO: Have this work with the factory once it's in the model.
    class RENDERERSHARED_EXPORT OpenGLFrameBuffer : public QOpenGLFramebufferObject
    {
        friend class FrameBufferFactory;
    public:
        OpenGLFrameBuffer(quint8 id, const QSize &size, GLenum target = GL_TEXTURE_2D);
        OpenGLFrameBuffer(quint8 id, const QSize &size, Attachment attachment, GLenum target = GL_TEXTURE_2D, GLenum internalFormat = 0);
        OpenGLFrameBuffer(quint8 id, const QSize &size, const QOpenGLFramebufferObjectFormat &format);

        quint8 factoryId() const;

    private:
        virtual ~OpenGLFrameBuffer();

        quint8 m_nId;
    };
}

#endif // OPENGLFRAMEBUFFER_H
