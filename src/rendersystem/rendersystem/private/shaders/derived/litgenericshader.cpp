#include "litgenericshader.h"
#include "rendersystem/private/shaders/common/vertexformat.h"

LitGenericShader::LitGenericShader(QObject* parent)
    : OpenGLShaderProgram(LitGenericShader::staticMetaObject.className(), parent)
{

}

LitGenericShader::~LitGenericShader()
{

}

RenderSystem::ShaderDefs::ShaderId LitGenericShader::shaderId()
{
    return RenderSystem::ShaderDefs::LitGenericShaderId;
}

void LitGenericShader::construct()
{
    addShaderFileWithCommonHeaders(QOpenGLShader::Vertex, ":/shaders/simplelitshader.vert");
    addShaderFileWithCommonHeaders(QOpenGLShader::Fragment, ":/shaders/simplelitshader.frag");
    link();
}

VertexFormat LitGenericShader::vertexFormat() const
{
    return VertexFormat(4, 3, 4, 2);
}

int LitGenericShader::maxBatchedItems() const
{
    return 8;
}
