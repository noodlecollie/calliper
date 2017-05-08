#ifndef OPENGLUNIFORMBUFFER_H
#define OPENGLUNIFORMBUFFER_H

#include <QOpenGLBuffer>

// TODO: Use the OpenGL types for arguments?
class OpenGLUniformBuffer
{
public:
    OpenGLUniformBuffer();
    OpenGLUniformBuffer(QOpenGLBuffer::UsagePattern pattern);
    OpenGLUniformBuffer(const OpenGLUniformBuffer &other);
    ~OpenGLUniformBuffer();

    inline bool operator ==(const OpenGLUniformBuffer &other)
    {
        return m_iHandle == other.m_iHandle;
    }

    GLuint bufferId() const;

    bool bind();
    bool bindToIndex(int bindingPoint);
    bool bindRange(int bindingPoint, quint32 offset, quint32 size);
    bool release();
    bool create();
    void destroy();
    bool isCreated() const;
    int size() const;

    bool allocate(const void* data, int count);
    bool allocate(int count);
    bool write(int offset, const void *data, int count);
    bool read(int offset, void *data, int count);
    void* map(QOpenGLBuffer::Access access);
    bool unmap();

    // This should not be called after allocate() or write().
    QOpenGLBuffer::UsagePattern usagePattern() const;
    void setUsagePattern(QOpenGLBuffer::UsagePattern pattern);

private:
    GLuint  m_iHandle;
    bool    m_bCreated;
    QOpenGLBuffer::UsagePattern m_iUsagePattern;
};

#endif // OPENGLUNIFORMBUFFER_H
