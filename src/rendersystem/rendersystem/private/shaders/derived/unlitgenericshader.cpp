#include "unlitgenericshader.h"
#include "rendersystem/private/shaders/common/vertexformat.h"

UnlitGenericShader::UnlitGenericShader(QObject* parent)
    : OpenGLShaderProgram(UnlitGenericShader::staticMetaObject.className(), parent)
{
}

UnlitGenericShader::~UnlitGenericShader()
{
}

RenderSystem::ShaderDefs::ShaderId UnlitGenericShader::shaderId()
{
    return RenderSystem::ShaderDefs::UnlitGenericShaderId;
}

void UnlitGenericShader::construct()
{
    addShaderFileWithCommonHeaders(QOpenGLShader::Vertex, ":/shaders/unlitshader.vert");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/unlitshader.frag");
    link();
}

VertexFormat UnlitGenericShader::vertexFormat() const
{
    return VertexFormat(4, 0, 4, 0);
}

int UnlitGenericShader::maxBatchedItems() const
{
    return 8;
}
