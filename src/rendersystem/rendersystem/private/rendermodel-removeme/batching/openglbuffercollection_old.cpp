#include "openglbuffercollection_old.h"

#if 0
#include "rendersystem/private/opengl/openglerrors.h"

OpenGLBufferCollection::OpenGLBufferCollection(QOpenGLBuffer::UsagePattern usagePattern)
    : m_nUsagePattern(usagePattern),
      m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_UniformBuffer(m_nUsagePattern)
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
    if ( isCreated() )
    {
        return true;
    }

    bool createResult = true;

    GLTRY(createResult = (createResult && m_VertexBuffer.create()));
    if ( !createResult )
    {
        m_VertexBuffer.destroy();
        return false;
    }

    GLTRY(createResult = (createResult && m_IndexBuffer.create()));
    if ( !createResult )
    {
        m_VertexBuffer.destroy();
        m_IndexBuffer.destroy();
        return false;
    }

    GLTRY(createResult = (createResult && m_UniformBuffer.create()));
    if ( !createResult )
    {
        m_VertexBuffer.destroy();
        m_IndexBuffer.destroy();
        m_UniformBuffer.destroy();
        return false;
    }

    return true;
}

void OpenGLBufferCollection::destroy()
{
    if ( !isCreated() )
    {
        return;
    }

    GLTRY(m_VertexBuffer.destroy());
    GLTRY(m_IndexBuffer.destroy());
    GLTRY(m_UniformBuffer.destroy());
}

bool OpenGLBufferCollection::isCreated() const
{
    return m_VertexBuffer.isCreated() && m_IndexBuffer.isCreated() && m_UniformBuffer.isCreated();
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
#endif
