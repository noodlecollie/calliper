#ifndef SIMPLELITSHADER_H
#define SIMPLELITSHADER_H

#include "model_global.h"
#include "renderer/opengl/openglshaderprogram.h"
#include "knownshaderdefs.h"

namespace Model
{
    class MODELSHARED_EXPORT SimpleLitShader : public Renderer::OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        SimpleLitShader(QObject* parent = 0);
        virtual ~SimpleLitShader();

        KnownShaderDefs::KnownShaderId knownShaderId() const;

        virtual void construct() override;
        virtual bool hasLocalUniformBlockBinding() const override;
        virtual Renderer::VertexFormat vertexFormat() const override;
        virtual int maxBatchedItems() const override;
    };
}

#endif // SIMPLELITSHADER_H
