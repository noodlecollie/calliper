#ifndef OPENGLCONTEXTCHECKER_H
#define OPENGLCONTEXTCHECKER_H

#include "calliperutil_global.h"

#include <QOpenGLContext>

namespace CalliperUtil
{
    class CALLIPERUTILSHARED_EXPORT OpenGLContextChecker
    {
    public:
        OpenGLContextChecker();

        void snapCreationContext();
        void clearCreationContext();

        bool verifyCurrentContext() const;

    private:
        quint64 m_nContextAddress;
    };
}

#endif // OPENGLCONTEXTCHECKER_H
