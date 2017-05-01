#ifndef OPENGLBUFFERCOLLECTION_H
#define OPENGLBUFFERCOLLECTION_H

#include <QOpenGLBuffer>
#include "rendersystem/private/opengl/opengluniformbuffer.h"

class OpenGLBufferCollection
{
public:
    OpenGLBufferCollection(QOpenGLBuffer::UsagePattern usagePattern);

    QOpenGLBuffer::UsagePattern usagePattern() const;

private:
    const QOpenGLBuffer::UsagePattern m_nUsagePattern;

    QOpenGLBuffer       m_VertexBuffer;
    QOpenGLBuffer       m_IndexBuffer;
    OpenGLUniformBuffer m_UniformBuffer;
};

#endif // OPENGLBATCH_H
