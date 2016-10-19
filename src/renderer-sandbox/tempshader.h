#ifndef TEMPSHADER_H
#define TEMPSHADER_H

#include "opengl/openglshaderprogram.h"

class TempShader : public NS_RENDERER::OpenGLShaderProgram
{
    Q_OBJECT
public:
    TempShader(quint16 id, QObject* parent = 0);
    virtual ~TempShader();

    virtual void construct() override;
    virtual bool hasLocalUniformBlockBinding() const override { return true; }

    virtual NS_RENDERER::VertexFormat vertexFormat() const override
    {
        return NS_RENDERER::VertexFormat(4, 0, 4, 2);
    }

    virtual int maxBatchedItems() const override
    {
        return 8;
    }
};

#endif // TEMPSHADER_H
