#ifndef SIMPLELITSHADER_H
#define SIMPLELITSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/private/shaders/common/batchedmodelmatrixuniformblock.h"

class SimpleLitShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    typedef BatchedModelMatrixUniformBlock<8> UniformBlock;

    SimpleLitShader(QObject* parent = 0);
    virtual ~SimpleLitShader();

    static PrivateShaderDefs::ShaderId shaderId();

    virtual void construct() override;
    virtual bool hasLocalUniformBlockBinding() const override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // SIMPLELITSHADER_H
