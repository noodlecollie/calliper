#include "openglbuffercollection.h"

#include "rendersystem/private/opengl/openglerrors.h"

OpenGLBufferCollection::OpenGLBufferCollection(QOpenGLBuffer::UsagePattern usagePattern)
    : m_nUsagePattern(usagePattern),
      m_VAO(),
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

    if ( !GLTRY_RET(m_VAO.create()) ||
         !GLTRY_RET(m_VertexBuffer.create()) ||
         !GLTRY_RET(m_IndexBuffer.create()) ||
         !GLTRY_RET(m_UniformBuffer.create()) )
    {
        destroyAll();
        return false;
    }

    return true;
}

void OpenGLBufferCollection::destroyAll()
{
    GLTRY(m_VertexBuffer.destroy());
    GLTRY(m_IndexBuffer.destroy());
    GLTRY(m_UniformBuffer.destroy());
    GLTRY(m_VAO.destroy());
}

void OpenGLBufferCollection::destroy()
{
    if ( !isCreated() )
    {
        return;
    }

    destroyAll();
}

bool OpenGLBufferCollection::isCreated() const
{
    return m_VAO.isCreated() && m_VertexBuffer.isCreated() && m_IndexBuffer.isCreated() && m_UniformBuffer.isCreated();
}