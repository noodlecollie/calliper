#ifndef SCREENSPACEQUADSHADER_H
#define SCREENSPACEQUADSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"

class ScreenSpaceQuadShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    explicit ScreenSpaceQuadShader(QObject* parent = Q_NULLPTR);
    virtual ~ScreenSpaceQuadShader();

    static PrivateShaderDefs::ShaderId shaderId();

    virtual void construct() override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // SCREENSPACEQUADSHADER_H
