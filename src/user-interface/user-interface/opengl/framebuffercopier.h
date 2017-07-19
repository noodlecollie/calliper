#ifndef FRAMEBUFFERCOPIER_H
#define FRAMEBUFFERCOPIER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class FrameBufferCopier
{
public:
    FrameBufferCopier();
    ~FrameBufferCopier();

    bool create();
    void destroy();
    bool isCreated() const;
    void draw(GLuint textureId);

    // TODO: Remove me, only a hack for debugging.
    quint64 creationContext() const;

private:
    void releaseAll();

    bool m_bCreatedSuccessfully;
    QOpenGLShaderProgram* m_pShaderProgram;
    GLuint m_VAOID;
    QOpenGLBuffer* m_pVertexBuffer;
    QOpenGLBuffer* m_pIndexBuffer;

    quint64 m_nCreationContext;
};

#endif // FRAMEBUFFERCOPIER_H
