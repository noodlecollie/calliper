#ifndef UNLITPERVERTEXCOLORSHADER_H
#define UNLITPERVERTEXCOLORSHADER_H

#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/private/shaders/common/batchedmodelmatrixuniformblock.h"

class UnlitPerVertexColorShader : public OpenGLShaderProgram
{
    Q_OBJECT
public:
    typedef BatchedModelMatrixUniformBlock<8> UniformBlock;

    UnlitPerVertexColorShader(QObject* parent = 0);
    virtual ~UnlitPerVertexColorShader();

    static PrivateShaderDefs::ShaderId shaderId();

    virtual void construct() override;
    virtual bool hasLocalUniformBlockBinding() const override;
    virtual VertexFormat vertexFormat() const override;
    virtual int maxBatchedItems() const override;
};

#endif // UNLITPERVERTEXCOLORSHADER_H
