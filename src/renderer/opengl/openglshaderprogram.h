#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H

#include "renderer_global.h"
#include <QOpenGLShaderProgram>
#include "shaders/ishaderspec.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT OpenGLShaderProgram : public QOpenGLShaderProgram, public IShaderSpec
    {
    public:
        // Subclasses should have a constructor which takes a quint16 ID and a default parent argument.
        OpenGLShaderProgram(quint16 id, const QString &name, QObject* parent = 0);
        virtual ~OpenGLShaderProgram();

        virtual void construct() = 0;

        // Should be bound before calling, and released after!
        virtual void setGlobalUniformBlockBinding();
        void setLocalUniformBlockBinding();
        virtual bool hasLocalUniformBlockBinding() const;

        void enableAttributeArrays();
        void disableAttributeArrays();

        quint16 shaderStoreId() const;

    private:
        const quint16 m_iShaderStoreId;
        GLuint m_iGlobalShaderBlockIndex;
        GLuint m_iLocalShaderBlockIndex;
    };
}

#endif // OPENGLSHADERPROGRAM_H
