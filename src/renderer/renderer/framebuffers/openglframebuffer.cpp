#include "openglframebuffer.h"

namespace Renderer
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(quint8 id, const QSize &size, Attachment attachment, GLenum target, GLenum internalFormat)
        : QOpenGLFramebufferObject(size, attachment, target, internalFormat),
          m_nId(id)
    {
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(quint8 id, const QSize &size, const QOpenGLFramebufferObjectFormat &format)
        : QOpenGLFramebufferObject(size, format),
          m_nId(id)
    {
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(quint8 id, const QSize &size, GLenum target)
        : QOpenGLFramebufferObject(size, target),
          m_nId(id)
    {
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
    }

    quint8 OpenGLFrameBuffer::factoryId() const
    {
        return m_nId;
    }
}
