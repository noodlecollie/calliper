#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H

#include "renderer_global.h"
#include <QOpenGLShaderProgram>
#include "ishaderspec.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT OpenGLShaderProgram : public QOpenGLShaderProgram, public IShaderSpec
    {
    public:
        OpenGLShaderProgram(QObject* parent = 0);
        virtual ~OpenGLShaderProgram();
    };
}

#endif // OPENGLSHADERPROGRAM_H
