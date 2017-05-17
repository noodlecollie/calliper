#ifndef OPENGLBUFFERCOLLECTION_OLD_H
#define OPENGLBUFFERCOLLECTION_OLD_H

#if 0
#include <QOpenGLBuffer>
#include "rendersystem/private/opengl/opengluniformbuffer.h"

class OpenGLBufferCollection
{
public:
    // TODO: Need a VAO per buffer collection.
    // It'd be nice to have one per vertex format, but
    // apparently VAOs remember the buffer they were
    // bound with, so there's no benefit to doing that.
    OpenGLBufferCollection(QOpenGLBuffer::UsagePattern usagePattern);

    QOpenGLBuffer::UsagePattern usagePattern() const;

    int batchSize() const;
    void setBatchSize(int size);

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

    int m_nBatchSize;
};
#endif

#endif // OPENGLBUFFERCOLLECTION_OLD_H
