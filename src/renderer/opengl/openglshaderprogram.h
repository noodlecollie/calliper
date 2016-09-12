#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H

#include "renderer_global.h"
#include <QOpenGLShaderProgram>
#include "shaders/ishaderspec.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT OpenGLShaderProgram : public QOpenGLShaderProgram, public IShaderSpec
    {
        friend class ShaderStore;
    public:
        OpenGLShaderProgram(const QString &name, QObject* parent = 0);
        virtual ~OpenGLShaderProgram();

        virtual void construct() = 0;

        // Should be bound before calling, and released after!
        virtual void setGlobalUniformBlockBinding();
        virtual void setLocalUniformBlockBinding();

        void enableAttributeArrays();
        void disableAttributeArrays();

        quint16 shaderStoreId() const;

    private:
        quint16 m_iShaderStoreId;
        GLuint m_iGlobalShaderBlockIndex;
    };
}

#endif // OPENGLSHADERPROGRAM_H
