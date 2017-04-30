#ifndef SIMPLELITSHADER_H
#define SIMPLELITSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/private/shaders/common/knownshaderdefs.h"

class SimpleLitShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    SimpleLitShader(QObject* parent = 0);
    virtual ~SimpleLitShader();

    KnownShaderDefs::KnownShaderId knownShaderId() const;

    virtual void construct() override;
    virtual bool hasLocalUniformBlockBinding() const override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // SIMPLELITSHADER_H
