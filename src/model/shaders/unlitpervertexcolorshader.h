#ifndef UNLITPERVERTEXCOLORSHADER_H
#define UNLITPERVERTEXCOLORSHADER_H

#include "model_global.h"
#include "opengl/openglshaderprogram.h"

namespace NS_MODEL
{
    class UnlitPerVertexColorShader : public NS_RENDERER::OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        UnlitPerVertexColorShader(quint16 id, QObject* parent = 0);
        virtual ~UnlitPerVertexColorShader();

        virtual void construct() override;
        virtual bool hasLocalUniformBlockBinding() const override;
        virtual NS_RENDERER::VertexFormat vertexFormat() const override;
        virtual int maxBatchedItems() const override;
    };
}

#endif // UNLITPERVERTEXCOLORSHADER_H
