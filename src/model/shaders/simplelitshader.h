#ifndef SIMPLELITSHADER_H
#define SIMPLELITSHADER_H

#include "model_global.h"
#include "opengl/openglshaderprogram.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT SimpleLitShader : public Renderer::OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        SimpleLitShader(quint16 id, QObject* parent = 0);
        virtual ~SimpleLitShader();

        virtual void construct() override;
        virtual bool hasLocalUniformBlockBinding() const override;
        virtual Renderer::VertexFormat vertexFormat() const override;
        virtual int maxBatchedItems() const override;
    };
}

#endif // SIMPLELITSHADER_H
