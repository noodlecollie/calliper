#include "openglvertexarrayobject.h"

#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

#include "rendersystem/private/shaders/common/vertexformat.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"
#include "rendersystem/global/rendersystem.h"

namespace
{
    template<typename LAMBDA>
    void callForNonZeroAttributes(const VertexFormat& format, LAMBDA lambda)
    {
        if ( format.positionComponents() > 0 )
        {
            lambda(PrivateShaderDefs::PositionAttribute);
        }

        if ( format.normalComponents() > 0 )
        {
            lambda(PrivateShaderDefs::NormalAttribute);
        }

        if ( format.colorComponents() > 0 )
        {
            lambda(PrivateShaderDefs::ColorAttribute);
        }

        if ( format.textureCoordinateComponents() > 0 )
        {
            lambda(PrivateShaderDefs::TextureCoordinateAttribute);
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
    checkContext();
    snapCreationContext();

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
    verifyCurrentContext();

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
    verifyCurrentContext();

    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    GL_CURRENT_F;
    bool success = GLTRY_RET(f->glBindVertexArray(m_iVAOID));
    Q_ASSERT(success);
    return success;
}

void OpenGLVertexArrayObject::release()
{
    verifyCurrentContext();

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
    verifyCurrentContext();

    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    if ( !isCreated() )
    {
        return;
    }

    GL_CURRENT_F;

    callForNonZeroAttributes(format, [&f](PrivateShaderDefs::VertexArrayAttribute attribute)
    {
        GLTRY(f->glEnableVertexAttribArray(attribute));
    });
}

void OpenGLVertexArrayObject::disableAttributeArrays(const VertexFormat &format)
{
    verifyCurrentContext();

    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    if ( !isCreated() )
    {
        return;
    }

    GL_CURRENT_F;

    callForNonZeroAttributes(format, [&f](PrivateShaderDefs::VertexArrayAttribute attribute)
    {
        GLTRY(f->glDisableVertexAttribArray(attribute));
    });
}

void OpenGLVertexArrayObject::disableAttributeArrays()
{
    verifyCurrentContext();

    Q_ASSERT_X(isCreated(), Q_FUNC_INFO, "VAO must be valid!");

    if ( !isCreated() )
    {
        return;
    }

    GL_CURRENT_F;

    for ( int attribute = 0; attribute < PrivateShaderDefs::TOTAL_VERTEX_ARRAY_ATTRIBUTES; ++attribute )
    {
        GLTRY(f->glDisableVertexAttribArray(attribute));
    }
}

void OpenGLVertexArrayObject::checkContext() const
{
    Q_ASSERT_X(RenderSystem::Global::renderSystemContextIsCurrent(), Q_FUNC_INFO, "Expected render system context to be current!");
}
