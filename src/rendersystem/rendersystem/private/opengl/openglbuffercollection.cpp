#include "openglbuffercollection.h"

#include "calliperutil/opengl/openglerrors.h"
#include "rendersystem/private/shaders/common/vertexformat.h"

namespace
{
    void trySetAttributeBuffer(OpenGLShaderProgram& shaderProgram,
                               PrivateShaderDefs::VertexArrayAttribute attribute,
                               int offset)
    {
        const int components = shaderProgram.vertexFormat().components(attribute);

        if ( components > 0 )
        {
            const int stride = shaderProgram.vertexFormat().totalVertexComponents() * sizeof(float);

            shaderProgram.setAttributeBuffer(attribute,
                                             GL_FLOAT,
                                             offset,
                                             components,
                                             stride);
        }
    }
}

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

void OpenGLBufferCollection::bindShaderAttributesToVAO(OpenGLShaderProgram &program)
{
    const VertexFormat format = program.vertexFormat();

    m_VAO.bind();
    m_VAO.disableAttributeArrays();
    m_VAO.enableAttributeArrays(program.vertexFormat());

    int offset = 0;

    for ( quint32 attributeInt = 0; attributeInt < PrivateShaderDefs::TOTAL_VERTEX_ARRAY_ATTRIBUTES; ++attributeInt )
    {
        const PrivateShaderDefs::VertexArrayAttribute attribute = static_cast<PrivateShaderDefs::VertexArrayAttribute>(attributeInt);
        trySetAttributeBuffer(program, attribute, offset);

        offset += format.components(attribute) * sizeof(float);
    }

    m_VAO.release();
}
