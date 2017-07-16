#include "openglvertexarrayobject.h"

#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

#include "rendersystem/private/shaders/common/vertexformat.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

namespace
{
    template<typename LAMBDA>
    void callForNonZeroAttributes(const VertexFormat& format, LAMBDA lambda)
    {
        if ( format.positionComponents() > 0 )
        {
            GLTRY(lambda(PrivateShaderDefs::PositionAttribute));
        }

        if ( format.normalComponents() > 0 )
        {
            GLTRY(lambda(PrivateShaderDefs::NormalAttribute));
        }

        if ( format.colorComponents() > 0 )
        {
            GLTRY(lambda(PrivateShaderDefs::ColorAttribute));
        }

        if ( format.textureCoordinateComponents() > 0 )
        {
            GLTRY(lambda(PrivateShaderDefs::TextureCoordinateAttribute));
        }
    }
}

OpenGLVertexArrayObject::OpenGLVertexArrayObject()
    : m_iVAOID(0)
{

}

OpenGLVertexArrayObject::~OpenGLVertexArrayObject()
{
    destroy();
}

bool OpenGLVertexArrayObject::create()
{
    if ( m_iVAOID != 0 )
    {
        return true;
    }

    GL_CURRENT_F;

    GLTRY(f->glGenVertexArrays(1, &m_iVAOID));
    return isCreated();
}

void OpenGLVertexArrayObject::destroy()
{
    if ( m_iVAOID == 0 )
    {
        return;
    }

    GL_CURRENT_F;
    GLTRY(f->glDeleteBuffers(1, &m_iVAOID));
    m_iVAOID = 0;
}

bool OpenGLVertexArrayObject::bind()
{
    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    GL_CURRENT_F;
    return GLTRY_RET(f->glBindVertexArray(m_iVAOID));
}

void OpenGLVertexArrayObject::release()
{
    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    GL_CURRENT_F;
    GLTRY(f->glBindVertexArray(0));
}

bool OpenGLVertexArrayObject::isCreated() const
{
    return m_iVAOID != 0;
}

void OpenGLVertexArrayObject::enableAttributeArrays(const VertexFormat &format)
{
    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    if ( !isCreated() )
    {
        return;
    }

    GL_CURRENT_F;

    callForNonZeroAttributes(format, [&f](PrivateShaderDefs::VertexArrayAttribute attribute)
    {
        f->glEnableVertexAttribArray(attribute);
    });
}

void OpenGLVertexArrayObject::disableAttributeArrays(const VertexFormat &format)
{
    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    if ( !isCreated() )
    {
        return;
    }

    GL_CURRENT_F;

    callForNonZeroAttributes(format, [&f](PrivateShaderDefs::VertexArrayAttribute attribute)
    {
        f->glDisableVertexAttribArray(attribute);
    });
}

void OpenGLVertexArrayObject::disableAttributeArrays()
{
    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    if ( !isCreated() )
    {
        return;
    }

    GL_CURRENT_F;

    for ( int attribute = 0; attribute < PrivateShaderDefs::TOTAL_VERTEX_ARRAY_ATTRIBUTES; ++attribute )
    {
        f->glDisableVertexAttribArray(attribute);
    }
}
