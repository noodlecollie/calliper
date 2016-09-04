#ifndef TEMPSHADER_H
#define TEMPSHADER_H

#include "opengl/openglshaderprogram.h"

class TempShader : public NS_RENDERER::OpenGLShaderProgram
{
    Q_OBJECT
public:
    TempShader(QObject* parent = 0);
    virtual ~TempShader();

    virtual void construct() override;

    virtual NS_RENDERER::VertexFormat vertexFormat() const override
    {
        return NS_RENDERER::VertexFormat(3, 0, 4, 2);
    }

    virtual int maxBatchedItems() const override
    {
        return 8;
    }
};

#endif // TEMPSHADER_H
