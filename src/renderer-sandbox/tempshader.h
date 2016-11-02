#ifndef TEMPSHADER_H
#define TEMPSHADER_H

#include "opengl/openglshaderprogram.h"

class TempShader : public Renderer::OpenGLShaderProgram
{
    Q_OBJECT
public:
    TempShader(quint16 id, QObject* parent = 0);
    virtual ~TempShader();

    virtual void construct() override;
    virtual bool hasLocalUniformBlockBinding() const override { return true; }

    virtual Renderer::VertexFormat vertexFormat() const override
    {
        return Renderer::VertexFormat(4, 0, 4, 2);
    }

    virtual int maxBatchedItems() const override
    {
        return 8;
    }
};

#endif // TEMPSHADER_H
