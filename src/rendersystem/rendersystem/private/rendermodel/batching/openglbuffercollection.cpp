#include "openglbuffercollection.h"

OpenGLBufferCollection::OpenGLBufferCollection(QOpenGLBuffer::UsagePattern usagePattern)
    : m_nUsagePattern(usagePattern),
      m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_UniformBuffer(m_nUsagePattern)
{
}

QOpenGLBuffer::UsagePattern OpenGLBufferCollection::usagePattern() const
{
    return m_nUsagePattern;
}
