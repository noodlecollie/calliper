#ifndef OPENGLERRORS_H
#define OPENGLERRORS_H

#include <QObject>
#include <QtOpenGL>

class OpenGLErrors
{
    Q_GADGET
public:
    enum OpenGLError
    {
        NoError                     = GL_NO_ERROR,
        InvalidEnum                 = GL_INVALID_ENUM,
        InvalidValue                = GL_INVALID_VALUE,
        InvalidOperation            = GL_INVALID_OPERATION,
        InvalidFrameBufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
        OutOfMemory                 = GL_OUT_OF_MEMORY,
        StackUnderflow              = GL_STACK_UNDERFLOW,
        StackOverflow               = GL_STACK_OVERFLOW,
    };
    Q_ENUM(OpenGLError)

    static QString errorString(GLenum errorCode);

    OpenGLErrors();
};

#endif // OPENGLERRORS_H
