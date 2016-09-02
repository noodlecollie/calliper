#include "opengluniformbuffer.h"
#include "openglhelpers.h"

namespace NS_RENDERER
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(QOpenGLBuffer::UsagePattern pattern)
        : m_iHandle(0),
          m_bCreated(false),
          m_iUsagePattern(pattern)
    {

    }

    OpenGLUniformBuffer::OpenGLUniformBuffer() : OpenGLUniformBuffer(QOpenGLBuffer::StaticDraw)
    {

    }

    OpenGLUniformBuffer::OpenGLUniformBuffer(const OpenGLUniformBuffer &other)
        : m_iHandle(other.m_iHandle),
          m_bCreated(other.m_bCreated),
          m_iUsagePattern(other.m_iUsagePattern)
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

        f->glGenBuffers(1, &m_iHandle);

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

        f->glDeleteBuffers(1, &m_iHandle);
        m_bCreated = false;
    }

    void OpenGLUniformBuffer::bind()
    {
        Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");

        GL_CURRENT_F;

        f->glBindBuffer(GL_UNIFORM_BUFFER, m_iHandle);
    }

    void OpenGLUniformBuffer::release()
    {
        Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");

        GL_CURRENT_F;

        f->glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

    void OpenGLUniformBuffer::allocate(const void *data, int count)
    {
        Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");

        GL_CURRENT_F;

        f->glBufferData(GL_UNIFORM_BUFFER, count, data, m_iUsagePattern);
    }

    void OpenGLUniformBuffer::allocate(int count)
    {
        allocate(NULL, count);
    }

    void OpenGLUniformBuffer::write(int offset, const void *data, int count)
    {
        Q_ASSERT_X(m_bCreated, Q_FUNC_INFO, "Buffer must be created first!");

        GL_CURRENT_F;

        f->glBufferSubData(GL_UNIFORM_BUFFER, offset, count, data);
    }

    bool OpenGLUniformBuffer::isCreated() const
    {
        return m_bCreated;
    }

    void OpenGLUniformBuffer::bindToIndex(int bindingPoint)
    {
        GL_CURRENT_F;

        f->glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_iHandle);
    }
}
