#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "opengl/openglshaderprogram.h"

class ColorShader : public NS_RENDERER::OpenGLShaderProgram
{
    Q_OBJECT
public:
    ColorShader(quint16 id, QObject* parent = 0);
    virtual ~ColorShader();

    virtual void construct() override;

    virtual NS_RENDERER::VertexFormat vertexFormat() const override
    {
        return NS_RENDERER::VertexFormat(3, 0, 4, 0);
    }

    virtual int maxBatchedItems() const override
    {
        return 8;
    }
};

#endif // COLORSHADER_H
