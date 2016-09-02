#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H

#include "renderer_global.h"
#include <QOpenGLShaderProgram>
#include "ishaderspec.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT OpenGLShaderProgram : public QOpenGLShaderProgram, public IShaderSpec
    {
        friend class ShaderStore;
    public:
        OpenGLShaderProgram(const QString &name, QObject* parent = 0);
        virtual ~OpenGLShaderProgram();

        virtual void construct() = 0;

        void enableAttributeArrays();
        void disableAttributeArrays();

        quint16 shaderStoreId() const;

    private:
        quint16 m_iShaderStoreId;
    };
}

#endif // OPENGLSHADERPROGRAM_H
