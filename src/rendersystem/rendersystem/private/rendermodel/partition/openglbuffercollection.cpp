#include "openglbuffercollection.h"

#include "calliperutil/opengl/openglerrors.h"

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

OpenGLBufferCollection::~OpenGLBufferCollection()
{
    destroy();
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

OpenGLVertexArrayObject& OpenGLBufferCollection::vertexArrayObject()
{
    return m_VAO;
}

const OpenGLVertexArrayObject& OpenGLBufferCollection::vertexArrayObject() const
{
    return m_VAO;
}

bool OpenGLBufferCollection::create()
{
    if ( isCreated() )
    {
        return true;
    }

    bool success = true;

    // If one step fails, short-circuiting should ensure no others are run.
    GLTRY(success = success && m_VAO.create());
    GLTRY(success = success && m_VertexBuffer.create());
    GLTRY(success = success && m_IndexBuffer.create());
    GLTRY(success = success && m_UniformBuffer.create());

    // VAO remembers which index buffer was bound, so we do that here.
    GLTRY(success = success && m_VAO.bind());
    GLTRY(success = success && m_IndexBuffer.bind());
    GLTRY(m_IndexBuffer.release());
    GLTRY(m_VAO.release());

    if ( !success )
    {
        destroyAll();
        return false;
    }

    return true;
}

void OpenGLBufferCollection::destroyAll()
{
    GLTRY(m_VertexBuffer.release());
    GLTRY(m_VertexBuffer.destroy());

    GLTRY(m_IndexBuffer.release());
    GLTRY(m_IndexBuffer.destroy());

    GLTRY(m_UniformBuffer.release());
    GLTRY(m_UniformBuffer.destroy());

    GLTRY(m_VAO.release());
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
