#include "opengluniformbuffer.h"
#include "openglhelpers.h"
#include "openglerrors.h"

OpenGLUniformBuffer::OpenGLUniformBuffer(QOpenGLBuffer::UsagePattern pattern)
    : m_iHandle(0),
      m_bCreated(false),
      m_iUsagePattern(pattern),
      m_bIsMapped(false)
{

}

OpenGLUniformBuffer::OpenGLUniformBuffer() : OpenGLUniformBuffer(QOpenGLBuffer::StaticDraw)
{

}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
    destroy();
}

bool OpenGLUniformBuffer::create()
{
    if ( m_bCreated )
        return true;

    GL_CURRENT_F;

    GLTRY(f->glGenBuffers(1, &m_iHandle));

    if ( m_iHandle == 0 )
        return false;

    m_bCreated = true;
    return true;
}

void OpenGLUniformBuffer::destroy()
{
    if ( !m_bCreated )
        return;

    GL_CURRENT_F;

    GLTRY(f->glDeleteBuffers(1, &m_iHandle));
    m_bCreated = false;
}

bool OpenGLUniformBuffer::bind()
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return false;
    }

    GL_CURRENT_F;

    return GLTRY_RET(f->glBindBuffer(GL_UNIFORM_BUFFER, m_iHandle));
}

bool OpenGLUniformBuffer::release()
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return false;
    }

    GL_CURRENT_F;

    return GLTRY_RET(f->glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

GLuint OpenGLUniformBuffer::bufferId() const
{
    return m_iHandle;
}

QOpenGLBuffer::UsagePattern OpenGLUniformBuffer::usagePattern() const
{
    return m_iUsagePattern;
}

void OpenGLUniformBuffer::setUsagePattern(QOpenGLBuffer::UsagePattern pattern)
{
    m_iUsagePattern = pattern;
}

bool OpenGLUniformBuffer::allocate(const void *data, int count)
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return false;
    }

    GL_CURRENT_F;

    return GLTRY_RET(f->glBufferData(GL_UNIFORM_BUFFER, count, data, m_iUsagePattern));
}

bool OpenGLUniformBuffer::allocate(int count)
{
    return allocate(Q_NULLPTR, count);
}

bool OpenGLUniformBuffer::write(int offset, const void *data, int count)
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return false;
    }

    GL_CURRENT_F;

    return GLTRY_RET(f->glBufferSubData(GL_UNIFORM_BUFFER, offset, count, data));
}

bool OpenGLUniformBuffer::read(int offset, void *data, int count)
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return false;
    }

    GL_CURRENT_F;

    return GLTRY_RET(f->glGetBufferSubData(GL_UNIFORM_BUFFER, offset, count, data));
}

bool OpenGLUniformBuffer::isCreated() const
{
    return m_bCreated;
}

bool OpenGLUniformBuffer::bindToIndex(int bindingPoint)
{
    GL_CURRENT_F;

    return GLTRY_RET(f->glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_iHandle)) && GLTRY_RET(bind());
}

bool OpenGLUniformBuffer::bindRange(int bindingPoint, quint32 offset, quint32 size)
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return false;
    }

    GL_CURRENT_F;

    return GLTRY_RET(f->glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_iHandle, offset, size)) && GLTRY_RET(bind());
}

int OpenGLUniformBuffer::size() const
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return -1;
    }

    GL_CURRENT_F;

    GLint value = -1;
    GLTRY(f->glGetBufferParameteriv(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &value));
    return value;
}

void* OpenGLUniformBuffer::map(QOpenGLBuffer::Access access)
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return Q_NULLPTR;
    }

    GL_CURRENT_F;

    void* ret = Q_NULLPTR;

    if ( !GLTRY_RET(ret = f->glMapBuffer(GL_UNIFORM_BUFFER, access)) )
    {
        ret = Q_NULLPTR;
    }

    m_bIsMapped = ret != Q_NULLPTR;
    return ret;
}

bool OpenGLUniformBuffer::unmap()
{
    Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");
    if ( !m_bCreated )
    {
        return false;
    }

    if ( !m_bIsMapped )
    {
        return true;
    }

    GL_CURRENT_F;

    bool ret = GLTRY_RET(f->glUnmapBuffer(GL_UNIFORM_BUFFER));
    m_bIsMapped = false;
    return ret;
}

bool OpenGLUniformBuffer::isMapped() const
{
    return m_bIsMapped;
}
