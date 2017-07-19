#ifndef OPENGLCONTEXTCHECKER_H
#define OPENGLCONTEXTCHECKER_H

#include <QOpenGLContext>

class OpenGLContextChecker
{
public:
    OpenGLContextChecker();

    void snapCreationContext();
    void clearCreationContext();

    bool verifyCurrentContext() const;

private:
    quint64 m_nContextAddress;
};

#endif // OPENGLCONTEXTCHECKER_H
