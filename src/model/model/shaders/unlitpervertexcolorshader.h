#ifndef UNLITPERVERTEXCOLORSHADER_H
#define UNLITPERVERTEXCOLORSHADER_H

#include "model_global.h"
#include "renderer/opengl/openglshaderprogram.h"
#include "knownshaderdefs.h"

namespace Model
{
    class MODELSHARED_EXPORT UnlitPerVertexColorShader : public Renderer::OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        UnlitPerVertexColorShader(QObject* parent = 0);
        virtual ~UnlitPerVertexColorShader();

        KnownShaderDefs::KnownShaderId knownShaderId() const;

        virtual void construct() override;
        virtual bool hasLocalUniformBlockBinding() const override;
        virtual Renderer::VertexFormat vertexFormat() const override;
        virtual int maxBatchedItems() const override;
    };
}

#endif // UNLITPERVERTEXCOLORSHADER_H
