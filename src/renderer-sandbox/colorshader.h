#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "opengl/openglshaderprogram.h"

class ColorShader : public Renderer::OpenGLShaderProgram
{
    Q_OBJECT
public:
    ColorShader(quint16 id, QObject* parent = 0);
    virtual ~ColorShader();

    virtual void construct() override;
    virtual bool hasLocalUniformBlockBinding() const override { return true; }

    virtual Renderer::VertexFormat vertexFormat() const override
    {
        return Renderer::VertexFormat(4, 0, 4, 0);
    }

    virtual int maxBatchedItems() const override
    {
        return 8;
    }
};

#endif // COLORSHADER_H
