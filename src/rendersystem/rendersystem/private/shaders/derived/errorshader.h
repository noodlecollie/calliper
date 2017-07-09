#ifndef ERRORSHADER_H
#define ERRORSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"

class ErrorShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    ErrorShader(QObject* parent = 0);
    virtual ~ErrorShader();

    static PrivateShaderDefs::ShaderId shaderId();

    virtual void construct() override;
    virtual bool hasLocalUniformBlockBinding() const override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // ERRORSHADER_H
