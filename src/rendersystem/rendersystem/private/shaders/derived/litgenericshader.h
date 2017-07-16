#ifndef LITGENERICSHADER_H
#define LITGENERICSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class LitGenericShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    LitGenericShader(QObject* parent = 0);
    virtual ~LitGenericShader();

    static RenderSystem::ShaderDefs::ShaderId shaderId();

    virtual void construct() override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // LITGENERICSHADER_H
