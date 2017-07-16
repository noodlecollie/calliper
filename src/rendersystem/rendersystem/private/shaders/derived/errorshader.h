#ifndef ERRORSHADER_H
#define ERRORSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class ErrorShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    explicit ErrorShader(QObject* parent = Q_NULLPTR);
    virtual ~ErrorShader();

    static RenderSystem::ShaderDefs::ShaderId shaderId();

    virtual void construct() override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // ERRORSHADER_H
