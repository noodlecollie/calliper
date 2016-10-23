#ifndef UNLITSHADER_H
#define UNLITSHADER_H

#include "model_global.h"
#include "opengl/openglshaderprogram.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT UnlitShader : public NS_RENDERER::OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        UnlitShader(quint16 id, QObject* parent = 0);
        virtual ~UnlitShader();

        virtual void construct() override;
        virtual bool hasLocalUniformBlockBinding() const override;
        virtual NS_RENDERER::VertexFormat vertexFormat() const override;
        virtual int maxBatchedItems() const override;
    };
}

#endif // UNLITSHADER_H
