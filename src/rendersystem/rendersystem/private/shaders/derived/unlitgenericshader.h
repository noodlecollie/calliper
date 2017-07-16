#ifndef UNLITGENERICSHADER_H
#define UNLITGENERICSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class UnlitGenericShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    UnlitGenericShader(QObject* parent = 0);
    virtual ~UnlitGenericShader();

    static RenderSystem::ShaderDefs::ShaderId shaderId();

    virtual void construct() override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // UNLITGENERICSHADER_H
