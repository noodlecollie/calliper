#ifndef DEBUGMINIMALSHADER_H
#define DEBUGMINIMALSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"

class DebugMinimalShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    explicit DebugMinimalShader(QObject* parent = Q_NULLPTR);
    virtual ~DebugMinimalShader();

    static PrivateShaderDefs::ShaderId shaderId();

    virtual void construct() override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // DEBUGMINIMALSHADER_H
