#ifndef UNLITSHADER_H
#define UNLITSHADER_H

#include "model_global.h"
#include "opengl/openglshaderprogram.h"

namespace Model
{
    class MODELSHARED_EXPORT UnlitShader : public Renderer::OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        UnlitShader(quint16 id, QObject* parent = 0);
        virtual ~UnlitShader();

        virtual void construct() override;
        virtual bool hasLocalUniformBlockBinding() const override;
        virtual Renderer::VertexFormat vertexFormat() const override;
        virtual int maxBatchedItems() const override;
    };
}

#endif // UNLITSHADER_H
