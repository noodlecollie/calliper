#ifndef FRAMEBUFFERCOPIER_H
#define FRAMEBUFFERCOPIER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "calliperutil/opengl/openglcontextchecker.h"

class FrameBufferCopier : private CalliperUtil::OpenGLContextChecker
{
public:
    FrameBufferCopier();
    ~FrameBufferCopier();

    bool create();
    void destroy();
    bool isCreated() const;
    void draw(GLuint textureId);

private:
    void releaseAll();

    bool m_bCreatedSuccessfully;
    QOpenGLShaderProgram* m_pShaderProgram;
    GLuint m_VAOID;
    QOpenGLBuffer* m_pVertexBuffer;
    QOpenGLBuffer* m_pIndexBuffer;
};

#endif // FRAMEBUFFERCOPIER_H
