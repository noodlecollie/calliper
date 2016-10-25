#ifndef ERRORSHADER_H
#define ERRORSHADER_H

#include "model_global.h"
#include "opengl/openglshaderprogram.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT ErrorShader : public NS_RENDERER::OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        ErrorShader(quint16 id, QObject* parent = 0);
        virtual ~ErrorShader();

        virtual void construct() override;
        virtual bool hasLocalUniformBlockBinding() const override;
        virtual NS_RENDERER::VertexFormat vertexFormat() const override;
        virtual int maxBatchedItems() const override;
    };
}

#endif // ERRORSHADER_H
