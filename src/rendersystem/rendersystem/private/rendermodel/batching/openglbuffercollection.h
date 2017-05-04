#ifndef OPENGLBUFFERCOLLECTION_H
#define OPENGLBUFFERCOLLECTION_H

#include <QOpenGLBuffer>
#include "rendersystem/private/opengl/opengluniformbuffer.h"

class OpenGLBufferCollection
{
public:
    // TODO: Probably need a VAO per vertex format.
    // Since we're interleaving attributes now, this doesn't
    // have to be per buffer any more.
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
    const QOpenGLBuffer::UsagePattern m_nUsagePattern;

    QOpenGLBuffer       m_VertexBuffer;
    QOpenGLBuffer       m_IndexBuffer;
    OpenGLUniformBuffer m_UniformBuffer;
    bool m_bCreated;
};

#endif // OPENGLBATCH_H
