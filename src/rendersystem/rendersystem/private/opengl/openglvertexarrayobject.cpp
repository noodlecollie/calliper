#include "openglvertexarrayobject.h"
#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

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
    GL_CURRENT_F;
    return GLTRY_RET(f->glBindVertexArray(m_iVAOID));
}

void OpenGLVertexArrayObject::release()
{
    GL_CURRENT_F;
    GLTRY(f->glBindVertexArray(0));
}

bool OpenGLVertexArrayObject::isCreated() const
{
    return m_iVAOID != 0;
}
