#ifndef OPENGLBUFFERCOLLECTION_H
#define OPENGLBUFFERCOLLECTION_H

#include <QOpenGLBuffer>

#include "rendersystem/private/opengl/opengluniformbuffer.h"
#include "rendersystem/private/opengl/openglvertexarrayobject.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class OpenGLBufferCollection
{
public:
    OpenGLBufferCollection(QOpenGLBuffer::UsagePattern usagePattern);

    QOpenGLBuffer::UsagePattern usagePattern() const;

    bool create();
    void destroy();
    bool isCreated() const;

    QOpenGLBuffer& vertexBuffer();
    const QOpenGLBuffer& vertexBuffer() const;

    QOpenGLBuffer& indexBuffer();
    const QOpenGLBuffer& indexBuffer() const;

    OpenGLUniformBuffer& uniformBuffer();
    const OpenGLUniformBuffer& uniformBuffer() const;

private:
    void destroyAll();

    const QOpenGLBuffer::UsagePattern m_nUsagePattern;

    OpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer       m_VertexBuffer;
    QOpenGLBuffer       m_IndexBuffer;
    OpenGLUniformBuffer m_UniformBuffer;
};

#endif // OPENGLBUFFERCOLLECTION_H
