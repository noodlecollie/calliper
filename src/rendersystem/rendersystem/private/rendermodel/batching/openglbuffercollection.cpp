#include "openglbuffercollection.h"

#include "rendersystem/private/opengl/openglerrors.h"

OpenGLBufferCollection::OpenGLBufferCollection(QOpenGLBuffer::UsagePattern usagePattern)
    : m_nUsagePattern(usagePattern),
      m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_UniformBuffer(m_nUsagePattern),
      m_bCreated(false)
{
    GLTRY(m_VertexBuffer.setUsagePattern(m_nUsagePattern));
    GLTRY(m_IndexBuffer.setUsagePattern(m_nUsagePattern));
    GLTRY(m_UniformBuffer.setUsagePattern(m_nUsagePattern));
}

QOpenGLBuffer::UsagePattern OpenGLBufferCollection::usagePattern() const
{
    return m_nUsagePattern;
}

QOpenGLBuffer& OpenGLBufferCollection::vertexBuffer()
{
    return m_VertexBuffer;
}

const QOpenGLBuffer& OpenGLBufferCollection::vertexBuffer() const
{
    return m_VertexBuffer;
}

QOpenGLBuffer& OpenGLBufferCollection::indexBuffer()
{
    return m_IndexBuffer;
}

const QOpenGLBuffer& OpenGLBufferCollection::indexBuffer() const
{
    return m_IndexBuffer;
}

OpenGLUniformBuffer& OpenGLBufferCollection::uniformBuffer()
{
    return m_UniformBuffer;
}

const OpenGLUniformBuffer& OpenGLBufferCollection::uniformBuffer() const
{
    return m_UniformBuffer;
}

bool OpenGLBufferCollection::create()
{
    if ( m_bCreated )
    {
        return true;
    }

    m_bCreated = true;

    GLTRY(m_bCreated = (m_bCreated && m_VertexBuffer.create()));
    if ( !m_bCreated )
    {
        m_VertexBuffer.destroy();

        m_bCreated = false;
        return m_bCreated;
    }

    GLTRY(m_bCreated = (m_bCreated && m_IndexBuffer.create()));
    if ( !m_bCreated )
    {
        m_VertexBuffer.destroy();
        m_IndexBuffer.destroy();

        m_bCreated = false;
        return m_bCreated;
    }

    GLTRY(m_bCreated = (m_bCreated && m_UniformBuffer.create()));
    if ( !m_bCreated )
    {
        m_VertexBuffer.destroy();
        m_IndexBuffer.destroy();
        m_UniformBuffer.destroy();

        m_bCreated = false;
        return m_bCreated;
    }

    return m_bCreated;
}

void OpenGLBufferCollection::destroy()
{
    if ( !m_bCreated )
    {
        return;
    }

    GLTRY(m_VertexBuffer.destroy());
    GLTRY(m_IndexBuffer.destroy());
    GLTRY(m_UniformBuffer.destroy());

    m_bCreated = false;
}

bool OpenGLBufferCollection::isCreated() const
{
    return m_bCreated;
}

int OpenGLBufferCollection::batchSize() const
{
    return m_nBatchSize;
}

void OpenGLBufferCollection::setBatchSize(int size)
{
    if ( size < 1 )
    {
        size = 1;
    }

    m_nBatchSize = size;
}
