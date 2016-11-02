#ifndef ERRORSHADER_H
#define ERRORSHADER_H

#include "model_global.h"
#include "opengl/openglshaderprogram.h"

namespace Model
{
    class MODELSHARED_EXPORT ErrorShader : public Renderer::OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        ErrorShader(quint16 id, QObject* parent = 0);
        virtual ~ErrorShader();

        virtual void construct() override;
        virtual bool hasLocalUniformBlockBinding() const override;
        virtual Renderer::VertexFormat vertexFormat() const override;
        virtual int maxBatchedItems() const override;
    };
}

#endif // ERRORSHADER_H
